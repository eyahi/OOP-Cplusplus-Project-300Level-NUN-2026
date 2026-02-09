/*
 * Author: Itoro Ifon, jason Hippolite, Prince Umeh
 * Date: 2026-01-22
 * Course/Assignment: C++ Project - Qt Music Player
 * File: MainWindow.h
 * Purpose: Declares the MainWindow class, which provides the user interface
 *          and connects UI actions to music playback logic.
 */
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDirIterator>
#include <QFileInfo>
#include <QHeaderView>
#include <QWidget>
#include <QStyle>
#include <QRegularExpression>
#include <QMenu>
#include <QDesktopServices>
#include <QUrl>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFile>
#include <QDir>

#include <QSettings>     //  session persistence
#include <QCloseEvent>   //  closeEvent override

#include <algorithm>

// Class: MusicPlayerWindow
// Purpose: Main UI window for the music player. Handles user interactions,
//          playlist display, searching, and controlling audio playback.
// Function: addSongs()
// Purpose: Opens a file dialog to let the user select audio files, validates
//          supported formats, then adds valid tracks to the playlist/list.
// Parameters: None
// Returns: void
// Notes: Currently supports WAV only.


// Source model columns
static constexpr int COL_TITLE  = 0;
static constexpr int COL_ARTIST = 1;
static constexpr int COL_LYRICS = 2; // hidden but searchable
static constexpr int COL_PATH   = 3; // hidden

static void showError(QWidget* parent, const QString& title, const QString& msg) {
    QMessageBox::warning(parent, title, msg);
}

// ===== Filter Title OR Artist OR Lyrics =====
bool TrackFilterModel::filterAcceptsRow(int row, const QModelIndex& parent) const {
    auto re = filterRegularExpression();
    if (!re.isValid() || re.pattern().isEmpty()) return true;

    for (int col : {COL_TITLE, COL_ARTIST, COL_LYRICS}) {
        QModelIndex idx = sourceModel()->index(row, col, parent);
        const QString text = sourceModel()->data(idx).toString();
        if (text.contains(re)) return true;
    }
    return false;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setAcceptDrops(true);

    buildUI();
    applyThemeLite();

    music.setVolume(70.f);
    volumeSlider->setValue(70);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::tick);
    timer->start(200);

    refreshPlayPauseIcon();
    updateCountLabel();

    // ✅ Restore last session (folder + track + position)
    restoreLastSession();
}

// ========================= UI =========================
void MainWindow::buildUI() {
    auto* central = new QWidget(this);
    setCentralWidget(central);

    openBtn = new QPushButton("Open Folder");
    openBtn->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
    connect(openBtn, &QPushButton::clicked, this, &MainWindow::openFolder);

    searchBox = new QLineEdit();
    searchBox->setPlaceholderText("Search by song name, artist, or lyrics…");
    searchBox->setClearButtonEnabled(true);

    countLabel = new QLabel("0 songs");
    countLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    auto* topRow = new QHBoxLayout();
    topRow->addWidget(openBtn);
    topRow->addSpacing(10);
    topRow->addWidget(new QLabel("Search:"));
    topRow->addWidget(searchBox, 1);
    topRow->addWidget(countLabel);

    model = new QStandardItemModel(this);
    model->setColumnCount(4);
    model->setHeaderData(COL_TITLE,  Qt::Horizontal, "Title");
    model->setHeaderData(COL_ARTIST, Qt::Horizontal, "Artist");
    model->setHeaderData(COL_LYRICS, Qt::Horizontal, "Lyrics");
    model->setHeaderData(COL_PATH,   Qt::Horizontal, "Path");

    proxy = new TrackFilterModel(this);
    proxy->setSourceModel(model);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

    table = new QTableView();
    table->setModel(proxy);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setShowGrid(false);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setHighlightSections(false);
    table->verticalHeader()->setDefaultSectionSize(30);

    table->setColumnHidden(COL_LYRICS, true);
    table->setColumnHidden(COL_PATH, true);

    connect(table, &QTableView::doubleClicked, this, &MainWindow::onDoubleClick);

    table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table, &QTableView::customContextMenuRequested, this, &MainWindow::onContextMenu);

    connect(searchBox, &QLineEdit::textChanged, this, [this](const QString& text) {
        QRegularExpression re(QRegularExpression::escape(text),
                              QRegularExpression::CaseInsensitiveOption);
        proxy->setFilterRegularExpression(re);
        updateCountLabel();
    });

    // ===== Mini Player Bar =====
    playerBar = new QFrame();
    playerBar->setObjectName("playerBar");

    artLabel = new QLabel();
    artLabel->setFixedSize(56, 56);

    bigTitleLabel = new QLabel("No song selected");
    bigTitleLabel->setObjectName("bigTitle");

    bigArtistLabel = new QLabel("—");
    bigArtistLabel->setObjectName("bigArtist");

    auto* metaCol = new QVBoxLayout();
    metaCol->setSpacing(2);
    metaCol->addWidget(bigTitleLabel);
    metaCol->addWidget(bigArtistLabel);

    prevBtn = new QPushButton();
    prevBtn->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    connect(prevBtn, &QPushButton::clicked, this, &MainWindow::prev);

    playPauseBtn = new QPushButton();
    connect(playPauseBtn, &QPushButton::clicked, this, &MainWindow::togglePlayPause);

    stopBtn = new QPushButton();
    stopBtn->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    connect(stopBtn, &QPushButton::clicked, this, &MainWindow::stop);

    nextBtn = new QPushButton();
    nextBtn->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    connect(nextBtn, &QPushButton::clicked, this, &MainWindow::next);

    auto* controlsRow = new QHBoxLayout();
    controlsRow->setSpacing(8);
    controlsRow->addWidget(prevBtn);
    controlsRow->addWidget(playPauseBtn);
    controlsRow->addWidget(stopBtn);
    controlsRow->addWidget(nextBtn);

    seekSlider = new QSlider(Qt::Horizontal);
    seekSlider->setRange(0, 1000);
    connect(seekSlider, &QSlider::sliderPressed, this, &MainWindow::seekPressed);
    connect(seekSlider, &QSlider::sliderReleased, this, &MainWindow::seekReleased);

    timeLabel = new QLabel("0:00 / 0:00");
    timeLabel->setMinimumWidth(90);
    timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    auto* seekRow = new QHBoxLayout();
    seekRow->addWidget(seekSlider, 1);
    seekRow->addWidget(timeLabel);

    volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    connect(volumeSlider, &QSlider::valueChanged, this, &MainWindow::volumeChanged);

    auto* volRow = new QHBoxLayout();
    volRow->addWidget(new QLabel("Volume"));
    volRow->addWidget(volumeSlider, 1);

    auto* rightCol = new QVBoxLayout();
    rightCol->setSpacing(8);
    rightCol->addLayout(controlsRow);
    rightCol->addLayout(seekRow);
    rightCol->addLayout(volRow);

    auto* barLayout = new QHBoxLayout(playerBar);
    barLayout->setContentsMargins(14, 12, 14, 12);
    barLayout->setSpacing(12);
    barLayout->addWidget(artLabel);
    barLayout->addLayout(metaCol, 1);
    barLayout->addLayout(rightCol, 2);

    auto* layout = new QVBoxLayout(central);
    layout->setSpacing(12);
    layout->setContentsMargins(14, 14, 14, 14);
    layout->addLayout(topRow);
    layout->addWidget(table, 1);
    layout->addWidget(playerBar);

    resize(980, 620);
    setArtworkPixmap(QPixmap());
}

void MainWindow::applyThemeLite() {
    setStyleSheet(R"(
QMainWindow { background: #1a2130; color: #eef2ff; }
QLabel { color: #eef2ff; }
QLineEdit {
    background: #232c3e;
    border: 1px solid #3b4a66;
    padding: 8px 10px;
    border-radius: 10px;
    color: #eef2ff;
}
QPushButton {
    background: #273149;
    border: 1px solid #3b4a66;
    padding: 8px 12px;
    border-radius: 10px;
    color: #eef2ff;
}
QPushButton:hover { background: #2d3954; }
QPushButton:pressed { background: #20283b; }
QTableView {
    background: #1f2740;
    border: 1px solid #3b4a66;
    border-radius: 14px;
    selection-background-color: #3b82f6;
    selection-color: white;
}
QHeaderView::section {
    background: #232c3e;
    color: #dbe3ff;
    padding: 10px;
    border: none;
}
QTableView::item { padding: 8px; }
QFrame#playerBar {
    background: #232c3e;
    border: 1px solid #3b4a66;
    border-radius: 18px;
}
QLabel#bigTitle { font-size: 16px; font-weight: 600; color: #f3f6ff; }
QLabel#bigArtist { font-size: 12px; color: #b8c3e6; }
QSlider::groove:horizontal { background: #3b4a66; height: 6px; border-radius: 3px; }
QSlider::handle:horizontal { background: #3b82f6; width: 14px; margin: -6px 0; border-radius: 7px; }
)");
}

void MainWindow::refreshPlayPauseIcon() {
    if (music.getStatus() == sf::Sound::Status::Playing)
        playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    else
        playPauseBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

// ========================= Drag & Drop =========================
void MainWindow::dragEnterEvent(QDragEnterEvent* e) {
    if (e->mimeData()->hasUrls()) e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent* e) {
    if (!e->mimeData()->hasUrls()) return;

    QStringList files;
    QString folder;
    QStringList unsupported;

    for (const auto& url : e->mimeData()->urls()) {
        const QString path = url.toLocalFile();
        if (path.isEmpty()) continue;

        QFileInfo fi(path);
        if (fi.isDir()) folder = path;
        else if (fi.isFile()) {
            if (isSupportedAudio(path)) files << path;
            else unsupported << fi.fileName();
        }
    }

    if (!unsupported.isEmpty()) {
        showError(this, "Unsupported files dropped",
                  "These files are not supported and were ignored:\n- " + unsupported.join("\n- ")
                      + "\n\nSupported: .wav .ogg .flac .aiff .au");
    }

    if (!folder.isEmpty()) { loadFolder(folder); return; }
    if (!files.isEmpty())  { addFiles(files); return; }
}

// ========================= Supported types =========================
bool MainWindow::isSupportedAudio(const QString& path) {
    QString ext = QFileInfo(path).suffix().toLower();
    return (ext == "wav" || ext == "ogg" || ext == "flac" || ext == "aiff" || ext == "au");
}

// ========================= Folder load =========================
void MainWindow::openFolder() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Music Folder");
    if (!dir.isEmpty()) loadFolder(dir);
}

void MainWindow::loadFolder(const QString& folderPath) {
    lastLoadedFolder = folderPath; // ✅ remember folder

    stoppedByUser = true;
    wasPlaying = false;
    music.stop();
    currentIndex = -1;

    playlistFullPaths.clear();
    model->removeRows(0, model->rowCount());
    searchBox->clear();

    bigTitleLabel->setText("No song selected");
    bigArtistLabel->setText("—");
    timeLabel->setText("0:00 / 0:00");
    seekSlider->setValue(0);
    setArtworkPixmap(QPixmap());
    refreshPlayPauseIcon();

    QDirIterator it(folderPath, QDir::Files);
    QStringList supportedPaths;
    QStringList unsupportedNames;

    while (it.hasNext()) {
        QString path = it.next();
        if (isSupportedAudio(path)) supportedPaths << path;
        else unsupportedNames << QFileInfo(path).fileName();
    }

    if (supportedPaths.isEmpty()) {
        showError(this, "No supported audio files",
                  "No supported audio files found in:\n" + folderPath +
                      "\n\nSupported: .wav .ogg .flac .aiff .au");
        updateCountLabel();
        return;
    }

    std::sort(supportedPaths.begin(), supportedPaths.end(), [](const QString& a, const QString& b) {
        return a.toLower() < b.toLower();
    });

    addFiles(supportedPaths);

    if (!unsupportedNames.isEmpty()) {
        QMessageBox::information(this, "Some files ignored",
                                 "Ignored unsupported files in this folder (examples):\n- "
                                     + unsupportedNames.mid(0, 12).join("\n- ")
                                     + (unsupportedNames.size() > 12 ? "\n..." : "")
                                     + "\n\nSupported: .wav .ogg .flac .aiff .au");
    }

    if (!playlistFullPaths.isEmpty()) {
        currentIndex = 0;
        QModelIndex srcIdx = model->index(0, 0);
        QModelIndex pxIdx = proxy->mapFromSource(srcIdx);
        if (pxIdx.isValid()) table->selectRow(pxIdx.row());
        updateNowPlaying();
        updateTimeUI();
    }
}

// ========================= Lyrics =========================
QString MainWindow::cleanLyricsText(QString s) {
    s.remove(QRegularExpression(R"(\[\d{1,2}:\d{2}(\.\d{1,2})?\])"));
    return s;
}

QString MainWindow::loadLyricsSidecar(const QString& audioPath) {
    QFileInfo fi(audioPath);
    QDir dir(fi.absolutePath());
    QString base = fi.completeBaseName();

    QStringList candidates = {
        dir.filePath(base + ".lrc"),
        dir.filePath(base + ".txt")
    };

    for (const auto& p : candidates) {
        QFile f(p);
        if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString text = QString::fromUtf8(f.readAll());
            return cleanLyricsText(text);
        }
    }
    return "";
}

// ========================= Add files =========================
void MainWindow::addFiles(const QStringList& filePaths) {
    QStringList failedToAdd;

    for (const auto& fullPath : filePaths) {
        if (!isSupportedAudio(fullPath)) continue;
        if (playlistFullPaths.contains(fullPath)) continue;

        if (!QFileInfo::exists(fullPath)) {
            failedToAdd << QFileInfo(fullPath).fileName();
            continue;
        }

        QFileInfo info(fullPath);
        QString baseName = info.completeBaseName();

        QString artist, title;
        parseArtistTitleFromFilename(baseName, artist, title);

        QString lyricsText = loadLyricsSidecar(fullPath);

        QList<QStandardItem*> row;
        row << new QStandardItem(title);
        row << new QStandardItem(artist);
        row << new QStandardItem(lyricsText);
        row << new QStandardItem(fullPath);

        model->appendRow(row);
        playlistFullPaths << fullPath;
    }

    updateCountLabel();

    if (!failedToAdd.isEmpty()) {
        showError(this, "Some files couldn't be added",
                  "These files were missing or inaccessible:\n- " + failedToAdd.join("\n- "));
    }
}

// ========================= Load a track =========================
bool MainWindow::loadIndex(int sourceRow) {
    if (sourceRow < 0 || sourceRow >= playlistFullPaths.size()) return false;

    const QString path = playlistFullPaths[sourceRow];

    if (!QFileInfo::exists(path)) {
        showError(this, "File missing",
                  "This file no longer exists:\n" + path +
                      "\n\nRemove it from playlist or reload folder.");
        return false;
    }

    music.stop();
    currentIndex = sourceRow;

    if (!music.openFromFile(path.toStdString())) {
        showError(this, "Playback failed",
                  "SFML could not open this file:\n" + path +
                      "\n\nPossible reasons:\n"
                      "- File is corrupted\n"
                      "- Unsupported codec inside the container\n"
                      "- Permission issues\n"
                      "\nTry converting it to WAV/OGG/FLAC again.");
        return false;
    }

    QModelIndex srcIdx = model->index(sourceRow, 0);
    QModelIndex pxIdx = proxy->mapFromSource(srcIdx);
    if (pxIdx.isValid()) table->selectRow(pxIdx.row());

    updateNowPlaying();
    updateTimeUI();
    return true;
}

// ========================= Playlist actions =========================
void MainWindow::onDoubleClick(const QModelIndex& index) {
    if (!index.isValid()) return;

    int sourceRow = proxy->mapToSource(index).row();
    if (!loadIndex(sourceRow)) return;

    music.play();
    stoppedByUser = false;
    refreshPlayPauseIcon();
}

void MainWindow::onContextMenu(const QPoint& pos) {
    QModelIndex proxyIdx = table->indexAt(pos);
    if (!proxyIdx.isValid()) return;

    int sourceRow = proxy->mapToSource(proxyIdx).row();
    if (sourceRow < 0 || sourceRow >= playlistFullPaths.size()) return;

    QMenu menu(this);
    QAction* actPlay     = menu.addAction("Play");
    QAction* actPlayNext = menu.addAction("Play Next");
    menu.addSeparator();
    QAction* actReveal   = menu.addAction("Reveal in Explorer");
    QAction* actRemove   = menu.addAction("Remove from Playlist");

    QAction* chosen = menu.exec(table->viewport()->mapToGlobal(pos));
    if (!chosen) return;

    if (chosen == actPlay) {
        if (!loadIndex(sourceRow)) return;
        music.play();
        stoppedByUser = false;
        refreshPlayPauseIcon();
        return;
    }

    if (chosen == actPlayNext) {
        if (currentIndex < 0) return;
        if (sourceRow == currentIndex || sourceRow == currentIndex + 1) return;

        QList<QStandardItem*> rowItems = model->takeRow(sourceRow);
        QString path = playlistFullPaths.takeAt(sourceRow);

        int insertPos = currentIndex + 1;
        if (insertPos > model->rowCount()) insertPos = model->rowCount();

        model->insertRow(insertPos, rowItems);
        playlistFullPaths.insert(insertPos, path);

        if (sourceRow < currentIndex) currentIndex -= 1;

        updateCountLabel();
        return;
    }

    if (chosen == actReveal) {
        QString path = playlistFullPaths[sourceRow];
        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(path).absolutePath()));
        return;
    }

    if (chosen == actRemove) {
        if (sourceRow == currentIndex) {
            stop();
            currentIndex = -1;
            bigTitleLabel->setText("No song selected");
            bigArtistLabel->setText("—");
            setArtworkPixmap(QPixmap());
        } else if (sourceRow < currentIndex) {
            currentIndex -= 1;
        }

        model->removeRow(sourceRow);
        playlistFullPaths.removeAt(sourceRow);
        updateCountLabel();
        return;
    }
}

// ========================= Controls =========================
void MainWindow::togglePlayPause() {
    if (playlistFullPaths.isEmpty()) {
        showError(this, "No songs", "Load a folder or drop audio files first.");
        return;
    }

    if (currentIndex < 0) {
        if (!loadIndex(0)) return;
    }

    if (music.getStatus() == sf::Sound::Status::Playing) {
        music.pause();
        stoppedByUser = false;
    } else {
        music.play();
        stoppedByUser = false;
    }

    refreshPlayPauseIcon();
    saveSession(true); // save immediately on play/pause
}

void MainWindow::stop() {
    music.stop();
    stoppedByUser = true;
    wasPlaying = false;
    refreshPlayPauseIcon();
    updateTimeUI();
    saveSession(true);
}

void MainWindow::next() {
    if (playlistFullPaths.isEmpty()) return;

    int last = playlistFullPaths.size() - 1;
    int nxt = std::min(currentIndex + 1, last);

    if (!loadIndex(nxt)) return;
    music.play();
    stoppedByUser = false;
    refreshPlayPauseIcon();
    saveSession(true);
}

void MainWindow::prev() {
    if (playlistFullPaths.isEmpty()) return;

    int prv = std::max(currentIndex - 1, 0);

    if (!loadIndex(prv)) return;
    music.play();
    stoppedByUser = false;
    refreshPlayPauseIcon();
    saveSession(true);
}

// ========================= Seek/Volume =========================
void MainWindow::seekPressed() { userSeeking = true; }

void MainWindow::seekReleased() {
    float dur = music.getDuration().asSeconds();
    if (dur <= 0.f) { userSeeking = false; return; }

    float target = (seekSlider->value() / 1000.f) * dur;
    music.setPlayingOffset(sf::seconds(target));

    userSeeking = false;
    updateTimeUI();
    saveSession(true);
}

void MainWindow::volumeChanged(int v) { music.setVolume((float)v); }

// ========================= Timer tick =========================
void MainWindow::tick() {
    auto st = music.getStatus();
    if (st == sf::Sound::Status::Playing) wasPlaying = true;

    // Auto-next when song ends naturally
    if (!playlistFullPaths.isEmpty() && currentIndex >= 0) {
        if (st == sf::Sound::Status::Stopped && !stoppedByUser && wasPlaying) {
            wasPlaying = false;

            if (currentIndex + 1 < playlistFullPaths.size()) {
                if (loadIndex(currentIndex + 1)) {
                    music.play();
                    refreshPlayPauseIcon();
                } else {
                    stoppedByUser = true;
                    refreshPlayPauseIcon();
                }
            } else {
                refreshPlayPauseIcon();
            }
        }
    }

    updateTimeUI();

    // ✅ Save session occasionally while playing (every ~1s here: 200ms * 5)
    tickCounter++;
    if (tickCounter >= 5) {
        tickCounter = 0;
        saveSession(false);
    }
}

// ========================= Counts & time =========================
void MainWindow::updateCountLabel() {
    int total = model->rowCount();
    int shown = proxy->rowCount();
    countLabel->setText(QString("Showing %1 of %2").arg(shown).arg(total));
}

QString MainWindow::formatTime(float seconds) {
    if (seconds < 0) seconds = 0;
    int total = (int)(seconds + 0.5f);
    int mins = total / 60;
    int secs = total % 60;
    return QString("%1:%2").arg(mins).arg(secs, 2, 10, QChar('0'));
}

void MainWindow::updateNowPlaying() {
    if (currentIndex < 0 || currentIndex >= playlistFullPaths.size()) return;

    QFileInfo info(playlistFullPaths[currentIndex]);
    QString baseName = info.completeBaseName();

    QString artist, title;
    parseArtistTitleFromFilename(baseName, artist, title);

    bigTitleLabel->setText(title.isEmpty() ? "Unknown Title" : title);
    bigArtistLabel->setText(artist.isEmpty() ? "Unknown Artist" : artist);

    setArtworkPixmap(loadArtworkForTrack(playlistFullPaths[currentIndex]));
}

void MainWindow::updateTimeUI() {
    float dur = music.getDuration().asSeconds();
    float pos = music.getPlayingOffset().asSeconds();

    if (dur < 0) dur = 0;
    if (pos < 0) pos = 0;
    if (pos > dur) pos = dur;

    timeLabel->setText(formatTime(pos) + " / " + formatTime(dur));

    if (!userSeeking && dur > 0.f) {
        int v = (int)((pos / dur) * 1000.f);
        v = std::max(0, std::min(1000, v));
        seekSlider->setValue(v);
    }
}

// ========================= Artwork =========================
QPixmap MainWindow::loadArtworkForTrack(const QString& audioPath) const {
    QFileInfo fi(audioPath);
    QDir dir(fi.absolutePath());

    QStringList coverNames = {"cover.jpg", "cover.jpeg", "cover.png", "folder.jpg", "folder.png"};
    for (const auto& name : coverNames) {
        QString p = dir.filePath(name);
        if (QFileInfo::exists(p)) {
            QPixmap px(p);
            if (!px.isNull()) return px;
        }
    }

    QString base = fi.completeBaseName();
    QStringList exts = {"jpg", "jpeg", "png"};
    for (const auto& ext : exts) {
        QString p = dir.filePath(base + "." + ext);
        if (QFileInfo::exists(p)) {
            QPixmap px(p);
            if (!px.isNull()) return px;
        }
    }

    return QPixmap();
}

void MainWindow::setArtworkPixmap(const QPixmap& px) {
    if (px.isNull()) {
        QPixmap placeholder(56, 56);
        placeholder.fill(QColor("#2b3754"));
        artLabel->setPixmap(placeholder);
        return;
    }

    QPixmap scaled = px.scaled(56, 56, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    artLabel->setPixmap(scaled);
}

// ========================= Filename parsing =========================
void MainWindow::parseArtistTitleFromFilename(const QString& fileNameNoExt, QString& artist, QString& title) {
    QString s = fileNameNoExt.trimmed();
    const QStringList seps = {" - ", " – ", " — "};

    for (const auto& sep : seps) {
        int p = s.indexOf(sep);
        if (p > 0) {
            artist = s.left(p).trimmed();
            title  = s.mid(p + sep.length()).trimmed();
            if (!title.isEmpty()) return;
        }
    }

    artist.clear();
    title = s;
}

// ========================= Session persistence =========================
void MainWindow::restoreLastSession() {
    QSettings s("NileUniversity", "QtMusicPlayer");

    const QString folder = s.value("player/lastFolder", "").toString();
    const int index      = s.value("player/lastIndex", -1).toInt();
    const double offset  = s.value("player/lastOffsetSeconds", 0.0).toDouble();
    const bool playNow   = s.value("player/wasPlaying", false).toBool();

    if (folder.isEmpty()) return;
    if (!QFileInfo::exists(folder)) return;

    loadFolder(folder);

    if (index >= 0 && index < playlistFullPaths.size()) {
        if (loadIndex(index)) {
            music.setPlayingOffset(sf::seconds(static_cast<float>(offset)));

            // If it was playing when user closed, resume. Otherwise keep paused.
            if (playNow) {
                stoppedByUser = false;
                music.play();
            } else {
                music.pause();
            }

            refreshPlayPauseIcon();
            updateTimeUI();
        }
    }
}

void MainWindow::saveSession(bool force) {
    // Need a folder to restore from
    if (lastLoadedFolder.isEmpty()) return;

    bool playing = (music.getStatus() == sf::Sound::Status::Playing);

    // If not forced, save only while playing (reduces registry writes)
    if (!force && !playing) return;

    QSettings s("NileUniversity", "QtMusicPlayer");
    s.setValue("player/lastFolder", lastLoadedFolder);
    s.setValue("player/lastIndex", currentIndex);
    s.setValue("player/lastOffsetSeconds", static_cast<double>(music.getPlayingOffset().asSeconds()));
    s.setValue("player/wasPlaying", playing);
}

void MainWindow::closeEvent(QCloseEvent* e) {
    saveSession(true);
    QMainWindow::closeEvent(e);
}
