#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include <QCloseEvent>

class QTableWidgetItem;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    explicit MainWindow(bool isAdmin, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Ensure data is saved when the window closes.
    void closeEvent(QCloseEvent *event) override;
    // Clear selection when clicking outside the table.
    bool eventFilter(QObject *object, QEvent *event) override;



private:
    Ui::MainWindow *ui;
    bool admin;
    // ---- UI setup helpers ----
    void initUi();
    void clearInputs();
    void populateInputsFromSelection();

    // ---- Validation and table helpers ----
    bool getInputValues(QString *id, QString *name, double *price, int *qty,
                        QString *errorMessage) const;
    int findRowById(const QString &id, int excludeRow = -1) const;
    void applyStockStyle(int row, int qty);
    bool ensureAdmin(const QString &action);
    QTableWidgetItem *ensureItem(int row, int column);
    void setRowValues(int row, const QString &id, const QString &name,
                      const QString &priceText, int qty);
    bool rowToText(int row, QString *id, QString *name,
                   QString *price, QString *qty) const;
    bool shouldIgnoreClear(QWidget *clicked) const;

private slots:
    // ---- Inventory actions ----
    void addProduct();
    void updateProduct();
    void deleteProduct();
    void saveToFile();
    void loadFromFile();
    void searchProduct();
    void exportReport();
    void logout();





};
#endif
