#include "BookingSystem.h"

void BookingSystem::addFlight(int id, QString dest, int seats) {
    flights.push_back(Flight(id, dest, seats));
}

QString BookingSystem::bookFlight(int id) {
    for (auto &f : flights) {
        if (f.id == id) {
            if (f.bookSeat()) return "Booking successful for Flight ID: " + QString::number(id);
            else return "No seats available!";
        }
    }
    return "Flight not found!";
}

QString BookingSystem::showFlights() {
    QString output;
    for (auto &f : flights) {
        output += "ID: " + QString::number(f.id)
                + " | Destination: " + f.destination
                + " | Seats: " + QString::number(f.seats) + "\n";
    }
    return output;
}
