
#include "MainWindow.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow() {

    //  Make window bigger
    resize(900, 600);

    QWidget *central = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout();

    flightId = new QLineEdit(); 
    flightId->setPlaceholderText("Flight ID");

    destination = new QLineEdit(); 
    destination->setPlaceholderText("Destination");

    seats = new QLineEdit(); 
    seats->setPlaceholderText("Seats");

    QPushButton *addFlightBtn = new QPushButton("Add Flight");

    bookId = new QLineEdit(); 
    bookId->setPlaceholderText("Flight ID to Book");

    QPushButton *bookBtn = new QPushButton("Book Flight");

    display = new QTextEdit(); 
    display->setReadOnly(true);

    layout->addWidget(flightId);
    layout->addWidget(destination);
    layout->addWidget(seats);
    layout->addWidget(addFlightBtn);
    layout->addWidget(bookId);
    layout->addWidget(bookBtn);
    layout->addWidget(display);

    central->setLayout(layout);
    setCentralWidget(central);

    connect(addFlightBtn, &QPushButton::clicked, [&]() {
        system.addFlight(
            flightId->text().toInt(),
            destination->text(),
            seats->text().toInt()
        );
        display->setText(system.showFlights());
    });

    connect(bookBtn, &QPushButton::clicked, [&]() {
        display->setText(
            system.bookFlight(bookId->text().toInt()) + "\n" +
            system.showFlights()
        );
    });
}
