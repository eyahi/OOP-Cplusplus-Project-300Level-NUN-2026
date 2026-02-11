#pragma once
#include <QMainWindow>

class QLineEdit;
class QTextEdit;
class QPushButton;
#include "BookingSystem.h"

// MainWindow handles the GUI
class MainWindow : public QMainWindow {
    Q_OBJECT

    BookingSystem system;    // Booking system object
    QLineEdit *flightId;     // Input for Flight ID
    QLineEdit *destination;  // Input for destination
    QLineEdit *seats;        // Input for seats
    QLineEdit *bookId;       // Input for booking flight ID
    QTextEdit *display;      // Display flights

public:
    MainWindow();
};
