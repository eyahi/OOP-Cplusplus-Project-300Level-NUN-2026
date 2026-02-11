#pragma once
#include "Flight.h"
#include <vector>
#include <QString>

// BookingSystem manages all flights and bookings
class BookingSystem {
private:
    std::vector<Flight> flights;  // List of flights

public:
    void addFlight(int id, QString dest, int seats);
    QString bookFlight(int id);
    QString showFlights();
};
