/*
 * Author: Itoro Ifon, jason Hippolite, Prince Umeh
 * Date: 2026-01-22
 * Course/Assignment: C++ Project - Qt Music Player
 * File: MainWindow.h
 * Purpose: Declares the MainWindow class, which provides the user interface
 *          and connects UI actions to music playback logic.
 */
#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QTableView>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QFrame>
#include <QStringList>
#include <QPixmap>

#include <SFML/Audio.hpp>

// Class: MusicPlayerWindow
// Purpose: Main UI window for the music player. Handles user interactions,
//          playlist display, searching, and controlling audio playback.
// Function: addSongs()
// Purpose: Opens a file dialog to let the user select audio files, validates
//          supported formats, then adds valid tracks to the playlist/list.
// Parameters: None
// Returns: void
// Notes: Currently supports WAV only.

class TrackFilterModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    using QSortFilterProxyModel::QSortFilterProxyModel;

protected:
    bool filterAcceptsRow(int row, const QModelIndex& parent) const override;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent* e) override;
    void dropEvent(QDropEvent* e) override;
    void closeEvent(QCloseEvent* e) override; // ✅ save on exit

private slots:
    void openFolder();
    void onDoubleClick(const QModelIndex& index);
    void onContextMenu(const QPoint& pos);

    void togglePlayPause();
    void stop();
    void next();
    void prev();

    void seekPressed();
    void seekReleased();
    void volumeChanged(int v);
    void tick();

private:
    // UI
    void buildUI();
    void applyThemeLite();
    void refreshPlayPauseIcon();

    // Library
    void loadFolder(const QString& folderPath);
    void addFiles(const QStringList& filePaths);
    bool loadIndex(int sourceRow);

    // UI updates
    void updateNowPlaying();
    void updateTimeUI();
    void updateCountLabel();

    // Helpers
    static QString formatTime(float seconds);
    static void parseArtistTitleFromFilename(const QString& fileNameNoExt, QString& artist, QString& title);
    static bool isSupportedAudio(const QString& path);

    static QString loadLyricsSidecar(const QString& audioPath);
    static QString cleanLyricsText(QString s);

    QPixmap loadArtworkForTrack(const QString& audioPath) const;
    void setArtworkPixmap(const QPixmap& px);

    // ✅ Session persistence
    void restoreLastSession();
    void saveSession(bool force = false);

    // ===== Widgets =====
    QPushButton* openBtn = nullptr;
    QLineEdit* searchBox = nullptr;
    QLabel* countLabel = nullptr;

    QTableView* table = nullptr;
    QStandardItemModel* model = nullptr;
    TrackFilterModel* proxy = nullptr;

    QFrame* playerBar = nullptr;
    QLabel* artLabel = nullptr;
    QLabel* bigTitleLabel = nullptr;
    QLabel* bigArtistLabel = nullptr;

    QPushButton* prevBtn = nullptr;
    QPushButton* playPauseBtn = nullptr;
    QPushButton* stopBtn = nullptr;
    QPushButton* nextBtn = nullptr;

    QSlider* seekSlider = nullptr;
    QLabel* timeLabel = nullptr;

    QSlider* volumeSlider = nullptr;
    QTimer* timer = nullptr;

    // ===== Audio =====
    sf::Music music;

    // Data: aligns with SOURCE model rows
    QStringList playlistFullPaths;

    // Playback state
    int currentIndex = -1;
    bool userSeeking = false;
    bool stoppedByUser = false;
    bool wasPlaying = false;

    // ✅ Keep track of folder we loaded (for session restore/save)
    QString lastLoadedFolder;

    // ✅ Throttle session saves
    int tickCounter = 0;
};
