#pragma once
#include <QString>

// Flight class represents a single flight
class Flight {
public:
    int id;              // Flight ID
    QString destination; // Flight destination
    int seats;           // Seats available

    Flight(int i, QString d, int s); // Constructor
    bool bookSeat();                 // Decrease seats by 1 if available
};
