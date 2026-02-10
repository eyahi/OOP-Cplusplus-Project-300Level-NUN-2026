#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "appdata.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>
#include <QHeaderView>
#include <QSaveFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QAbstractItemView>
#include <QBrush>
#include <QColor>
#include <QLocale>
#include <QStandardPaths>
#include <QDir>
#include <QMouseEvent>
#include <QEvent>
#include <QStringList>
#include <QtGlobal>
#include "loginwindow.h"

namespace {
// Table layout and visual rules.
enum Column { ColId = 0, ColName, ColPrice, ColQty, ColumnCount };
const int kLowStockThreshold = 10;
const QColor kLowStockColor(180, 60, 60);
const QStringList kHeaders = {"ID", "Name", "Price", "Quantity"};
}



// Default constructor delegates to the admin-aware constructor.
MainWindow::MainWindow(QWidget *parent)
    : MainWindow(false, parent)
{
}

// Main constructor: sets role, builds UI, and loads data.
MainWindow::MainWindow(bool isAdmin, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , admin(isAdmin)
{
    initUi();
    loadFromFile();
}






MainWindow::~MainWindow()
{
    // Clean up the UI.
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Save inventory before the window closes.
    saveToFile();
    QMainWindow::closeEvent(event);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object);
    // When clicking outside the table, clear selection and input fields.
    if (event->type() == QEvent::MouseButtonPress) {
        auto *mouseEvent = static_cast<QMouseEvent *>(event);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        const QPoint globalPos = mouseEvent->globalPosition().toPoint();
#else
        const QPoint globalPos = mouseEvent->globalPos();
#endif
        QWidget *clicked = QApplication::widgetAt(globalPos);
        if (shouldIgnoreClear(clicked)) {
            return QMainWindow::eventFilter(object, event);
        }
        const QPoint viewportPos = ui->tableWidget->viewport()->mapFromGlobal(globalPos);
        const bool inViewport = ui->tableWidget->viewport()->rect().contains(viewportPos);

        if (!inViewport) {
            ui->tableWidget->clearSelection();
            clearInputs();
        } else if (!ui->tableWidget->indexAt(viewportPos).isValid()) {
            ui->tableWidget->clearSelection();
            clearInputs();
        }
    }

    return QMainWindow::eventFilter(object, event);
}

void MainWindow::initUi()
{
    // Build the main UI from the .ui file.
    ui->setupUi(this);

    // ---- Table setup ----
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(ColumnCount);
    ui->tableWidget->setHorizontalHeaderLabels(kHeaders);
    ui->tableWidget->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    qApp->installEventFilter(this);

    // ---- Input validators ----
    auto *priceValidator = new QDoubleValidator(0.0, 1000000.0, 2, ui->priceInput);
    priceValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->priceInput->setValidator(priceValidator);
    ui->qtyInput->setValidator(new QIntValidator(0, 1000000, ui->qtyInput));
    ui->idInput->setMaxLength(32);
    ui->nameInput->setMaxLength(64);
    ui->searchInput->setClearButtonEnabled(true);

    // ---- Signal hookups ----
    connect(ui->addBtn, &QPushButton::clicked,
            this, &MainWindow::addProduct);

    connect(ui->searchInput, &QLineEdit::textChanged,
            this, &MainWindow::searchProduct);

    connect(ui->updateBtn, &QPushButton::clicked,
            this, &MainWindow::updateProduct);

    connect(ui->deleteBtn, &QPushButton::clicked,
            this, &MainWindow::deleteProduct);

    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::populateInputsFromSelection);

    connect(qApp, &QApplication::aboutToQuit,
            this, &MainWindow::saveToFile);

    connect(ui->exportBtn, &QPushButton::clicked,
            this, &MainWindow::exportReport);
    connect(ui->logoutBtn, &QPushButton::clicked,
            this, &MainWindow::logout);

    // ---- Role-based UI lock ----
    if (!admin) {
        ui->addBtn->setEnabled(false);
        ui->updateBtn->setEnabled(false);
        ui->deleteBtn->setEnabled(false);
        ui->idInput->setReadOnly(true);
        ui->nameInput->setReadOnly(true);
        ui->priceInput->setReadOnly(true);
        ui->qtyInput->setReadOnly(true);
    }

    // ---- Simple dark theme ----
    this->setStyleSheet(
        "QMainWindow{background:#121212;}"
        "QLineEdit{padding:6px;border-radius:6px;}"
        "QPushButton{background:#1f1f1f;color:white;"
        "padding:8px;border-radius:8px;}"
        "QPushButton:hover{background:#333;}"
        "QTableWidget{background:#1a1a1a;color:white;}"
        );
}

bool MainWindow::getInputValues(QString *id, QString *name, double *price, int *qty,
                                QString *errorMessage) const
{
    // Pull raw text from inputs.
    const QString rawId = ui->idInput->text().trimmed();
    const QString rawName = ui->nameInput->text().trimmed();
    const QString rawPrice = ui->priceInput->text().trimmed();
    const QString rawQty = ui->qtyInput->text().trimmed();

    // Basic required-field checks.
    if (rawId.isEmpty() || rawName.isEmpty() || rawPrice.isEmpty() || rawQty.isEmpty()) {
        if (errorMessage) {
            *errorMessage = "All fields are required.";
        }
        return false;
    }

    // CSV safety checks (no commas/newlines in text fields).
    if (rawId.contains(",") || rawName.contains(",") ||
        rawId.contains("\n") || rawName.contains("\n")) {
        if (errorMessage) {
            *errorMessage = "Commas and newlines are not allowed in ID or name.";
        }
        return false;
    }

    bool priceOk = false;
    bool qtyOk = false;
    const double parsedPrice = QLocale::c().toDouble(rawPrice, &priceOk);
    const int parsedQty = rawQty.toInt(&qtyOk);

    // Numeric validation.
    if (!priceOk || parsedPrice < 0.0) {
        if (errorMessage) {
            *errorMessage = "Price must be a valid non-negative number.";
        }
        return false;
    }

    if (!qtyOk || parsedQty < 0) {
        if (errorMessage) {
            *errorMessage = "Quantity must be a valid non-negative number.";
        }
        return false;
    }

    if (id) {
        *id = rawId;
    }
    if (name) {
        *name = rawName;
    }
    if (price) {
        *price = parsedPrice;
    }
    if (qty) {
        *qty = parsedQty;
    }
    return true;
}

bool MainWindow::ensureAdmin(const QString &action)
{
    // Guard all write operations for admins only.
    if (admin) {
        return true;
    }
    QMessageBox::warning(this,
                         "Not Allowed",
                         QString("Only admins can %1 products.").arg(action));
    return false;
}

QTableWidgetItem *MainWindow::ensureItem(int row, int column)
{
    // Ensure a cell exists so we can safely write text into it.
    QTableWidgetItem *item = ui->tableWidget->item(row, column);
    if (!item) {
        item = new QTableWidgetItem();
        ui->tableWidget->setItem(row, column, item);
    }
    return item;
}

void MainWindow::setRowValues(int row, const QString &id, const QString &name,
                              const QString &priceText, int qty)
{
    // Write a full row in one place.
    ensureItem(row, ColId)->setText(id);
    ensureItem(row, ColName)->setText(name);
    ensureItem(row, ColPrice)->setText(priceText);
    ensureItem(row, ColQty)->setText(QString::number(qty));
    applyStockStyle(row, qty);
}

bool MainWindow::rowToText(int row, QString *id, QString *name,
                           QString *price, QString *qty) const
{
    // Read a full row; return false if any cell is missing.
    QTableWidgetItem *idItem = ui->tableWidget->item(row, ColId);
    QTableWidgetItem *nameItem = ui->tableWidget->item(row, ColName);
    QTableWidgetItem *priceItem = ui->tableWidget->item(row, ColPrice);
    QTableWidgetItem *qtyItem = ui->tableWidget->item(row, ColQty);

    if (!idItem || !nameItem || !priceItem || !qtyItem) {
        return false;
    }

    if (id) {
        *id = idItem->text();
    }
    if (name) {
        *name = nameItem->text();
    }
    if (price) {
        *price = priceItem->text();
    }
    if (qty) {
        *qty = qtyItem->text();
    }
    return true;
}

int MainWindow::findRowById(const QString &id, int excludeRow) const
{
    // Find a row by product ID (optionally skipping a row).
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        if (i == excludeRow) {
            continue;
        }
        QTableWidgetItem *item = ui->tableWidget->item(i, ColId);
        if (item && item->text().trimmed() == id) {
            return i;
        }
    }
    return -1;
}

void MainWindow::applyStockStyle(int row, int qty)
{
    // Highlight low stock rows in red.
    const bool low = qty <= kLowStockThreshold;
    for (int col = 0; col < ColumnCount; ++col) {
        QTableWidgetItem *item = ui->tableWidget->item(row, col);
        if (!item) {
            continue;
        }
        if (low) {
            item->setBackground(kLowStockColor);
        } else {
            item->setBackground(QBrush());
        }
    }
}

void MainWindow::clearInputs()
{
    // Clear the input fields at the top.
    ui->idInput->clear();
    ui->nameInput->clear();
    ui->priceInput->clear();
    ui->qtyInput->clear();
}

void MainWindow::populateInputsFromSelection()
{
    // When a row is selected, mirror its values into the inputs.
    const int row = ui->tableWidget->currentRow();
    if (row < 0) {
        clearInputs();
        return;
    }

    QTableWidgetItem *idItem = ui->tableWidget->item(row, ColId);
    QTableWidgetItem *nameItem = ui->tableWidget->item(row, ColName);
    QTableWidgetItem *priceItem = ui->tableWidget->item(row, ColPrice);
    QTableWidgetItem *qtyItem = ui->tableWidget->item(row, ColQty);

    if (idItem) ui->idInput->setText(idItem->text());
    if (nameItem) ui->nameInput->setText(nameItem->text());
    if (priceItem) ui->priceInput->setText(priceItem->text());
    if (qtyItem) ui->qtyInput->setText(qtyItem->text());
}

bool MainWindow::shouldIgnoreClear(QWidget *clicked) const
{
    // Don't clear inputs when clicking buttons or the input fields themselves.
    return clicked == ui->addBtn ||
           clicked == ui->updateBtn ||
           clicked == ui->deleteBtn ||
           clicked == ui->exportBtn ||
           clicked == ui->logoutBtn ||
           clicked == ui->idInput ||
           clicked == ui->nameInput ||
           clicked == ui->priceInput ||
           clicked == ui->qtyInput;
}

void MainWindow::addProduct()
{
    // Add a new product row from inputs.
    if (!ensureAdmin("add")) {
        return;
    }

    QString error;
    QString id;
    QString name;
    double price = 0.0;
    int qty = 0;
    if (!getInputValues(&id, &name, &price, &qty, &error)) {
        QMessageBox::warning(this, "Error", error);
        return;
    }

    // Prevent duplicate IDs.
    if (findRowById(id) != -1) {
        QMessageBox::warning(this, "Error", "Product ID already exists.");
        return;
    }

    const int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    const QString priceText = QLocale::c().toString(price, 'f', 2);
    setRowValues(row, id, name, priceText, qty);

    clearInputs();
    searchProduct();
    ui->idInput->setFocus();
}



void MainWindow::updateProduct()
{
    // Update the currently selected row.
    if (!ensureAdmin("update")) {
        return;
    }

    const int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Select a product to update.");
        return;
    }

    QString error;
    QString id;
    QString name;
    double price = 0.0;
    int qty = 0;
    if (!getInputValues(&id, &name, &price, &qty, &error)) {
        QMessageBox::warning(this, "Error", error);
        return;
    }

    if (findRowById(id, row) != -1) {
        QMessageBox::warning(this, "Error", "Product ID already exists.");
        return;
    }

    const QString priceText = QLocale::c().toString(price, 'f', 2);
    setRowValues(row, id, name, priceText, qty);
    searchProduct();
}

void MainWindow::deleteProduct()
{
    // Delete the currently selected row.
    if (!ensureAdmin("delete")) {
        return;
    }

    const int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Select a product to delete.");
        return;
    }

    const auto reply = QMessageBox::question(this,
                                             "Confirm Delete",
                                             "Delete the selected product?");
    if (reply != QMessageBox::Yes) {
        return;
    }

    ui->tableWidget->removeRow(row);
    searchProduct();
}


void MainWindow::loadFromFile()
{
    // Load inventory from AppData.
    const QString pathToOpen = AppData::inventoryFilePath();

    if (!QFileInfo::exists(pathToOpen)) {
        return;
    }

    QFile file(pathToOpen);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open inventory file.");
        return;
    }

    QTextStream in(&file);
    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->setRowCount(0);

    // Parse CSV rows.
    while(!in.atEnd()) {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }

        const QStringList data = line.split(",");
        if (data.size() < 4) {
            continue;
        }

        const QString header = data[0].trimmed().toLower();
        if (header == "id") {
            continue;
        }

        const QString id = data[0].trimmed();
        const QString name = data[1].trimmed();
        const QString priceText = data[2].trimmed();
        const QString qtyText = data[3].trimmed();

        if (id.isEmpty() || name.isEmpty()) {
            continue;
        }

        if (findRowById(id) != -1) {
            continue;
        }

        bool priceOk = false;
        bool qtyOk = false;
        const double price = QLocale::c().toDouble(priceText, &priceOk);
        const int qty = qtyText.toInt(&qtyOk);
        if (!priceOk || !qtyOk) {
            continue;
        }

        const int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        setRowValues(row, id, name,
                     QLocale::c().toString(price, 'f', 2),
                     qty);
    }

    ui->tableWidget->setSortingEnabled(true);
    file.close();
}

void MainWindow::saveToFile()
{
    // Only admins can persist inventory.
    if(!admin) return;

    // Ensure AppData directory exists.
    QString dirError;
    if (!AppData::ensureDataDir(&dirError)) {
        return;
    }

    // Write CSV to the primary inventory file.
    QSaveFile file(AppData::inventoryFilePath());
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    out << "id,name,price,quantity\n";

    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QString id;
        QString name;
        QString price;
        QString qty;
        if (!rowToText(i, &id, &name, &price, &qty)) {
            continue;
        }

        id = id.trimmed();
        name = name.trimmed();
        price = price.trimmed();
        qty = qty.trimmed();

        if (id.isEmpty() || name.isEmpty()) {
            continue;
        }

        out << id << "," << name << ","
            << price << "," << qty << "\n";
    }

    file.commit();
}


void MainWindow::searchProduct()
{
    // Filter rows based on search text.
    const QString text = ui->searchInput->text().trimmed().toLower();
    const bool hasFilter = !text.isEmpty();

    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        bool match = !hasFilter;

        if (hasFilter) {
            for(int j = 0; j < ColumnCount; j++)
            {
                QTableWidgetItem *item = ui->tableWidget->item(i,j);
                if(item && item->text().toLower().contains(text)) {
                    match = true;
                    break;
                }
            }
        }

        ui->tableWidget->setRowHidden(i, !match);
    }
}
void MainWindow::exportReport()
{
    // Export a human-readable report to a user-selected file.
    if (ui->tableWidget->rowCount() == 0) {
        QMessageBox::information(this, "No Data", "There are no products to export.");
        return;
    }

    const QString defaultDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    const QString defaultPath = defaultDir.isEmpty()
        ? QString("inventory_report.txt")
        : defaultDir + QDir::separator() + "inventory_report.txt";

    const QString path = QFileDialog::getSaveFileName(
        this,
        "Export Report",
        defaultPath,
        "Text Files (*.txt)");

    if (path.isEmpty()) {
        return;
    }

    QSaveFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,
                             "Error",
                             "Could not create report file!");
        return;
    }

    QTextStream out(&file);

    out << "SUPERMARKET INVENTORY REPORT\n\n";

    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        QString id;
        QString name;
        QString price;
        QString qty;
        if (!rowToText(i, &id, &name, &price, &qty)) {
            continue;
        }

        out << "ID: " << id << "\n";
        out << "Name: " << name << "\n";
        out << "Price: " << price << "\n";
        out << "Qty: " << qty << "\n\n";
    }

    file.commit();

    QMessageBox::information(this,
                             "Export Successful",
                             "Report exported successfully!");
}

void MainWindow::logout()
{
    // Save and return to the login screen.
    saveToFile();
    auto *login = new LoginWindow;
    login->setAttribute(Qt::WA_DeleteOnClose);
    login->show();
    close();
}

