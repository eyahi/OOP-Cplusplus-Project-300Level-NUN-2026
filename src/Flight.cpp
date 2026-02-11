/**
 * @file Flight.cpp
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Implementation of Flight class
 */

#include "Flight.h"
#include <iomanip>
#include <sstream>

Flight::Flight(int id, const std::string& name, double price,
               const std::string& airline, const std::string& origin,
               const std::string& destination, const std::string& departureTime,
               const std::string& arrivalTime, int totalSeats,
               const std::string& flightNumber)
    : Bookable(id, name, price, origin),
      airline(airline), origin(origin), destination(destination),
      departureTime(departureTime), arrivalTime(arrivalTime),
      totalSeats(totalSeats), availableSeats(totalSeats),
      flightNumber(flightNumber) {}

std::string Flight::getAirline() const {
    return airline;
}

std::string Flight::getOrigin() const {
    return origin;
}

std::string Flight::getDestination() const {
    return destination;
}

std::string Flight::getDepartureTime() const {
    return departureTime;
}

std::string Flight::getArrivalTime() const {
    return arrivalTime;
}

int Flight::getAvailableSeats() const {
    return availableSeats;
}

int Flight::getTotalSeats() const {
    return totalSeats;
}

std::string Flight::getFlightNumber() const {
    return flightNumber;
}

bool Flight::bookSeats(int numSeats) {
    if (numSeats <= 0 || numSeats > availableSeats) {
        return false;
    }
    availableSeats -= numSeats;
    return true;
}

void Flight::cancelSeats(int numSeats) {
    availableSeats += numSeats;
    if (availableSeats > totalSeats) {
        availableSeats = totalSeats;
    }
}

void Flight::displayInfo() const {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                      ✈️  FLIGHT DETAILS                       ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Flight ID: " << std::setw(10) << id 
              << "        Flight #: " << std::setw(15) << flightNumber << "   ║\n";
    std::cout << "║ Airline: " << std::setw(52) << std::left << airline << "║\n" << std::right;
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ From: " << std::setw(20) << std::left << origin 
              << " ───➤ To: " << std::setw(20) << destination << "  ║\n" << std::right;
    std::cout << "║ Departure: " << std::setw(20) << std::left << departureTime 
              << " Arrival: " << std::setw(18) << arrivalTime << "  ║\n" << std::right;
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Price: $" << std::fixed << std::setprecision(2) << std::setw(10) << price
              << "        Available Seats: " << std::setw(5) << availableSeats 
              << "/" << std::setw(5) << totalSeats << "   ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

std::string Flight::getType() const {
    return "Flight";
}

bool Flight::isAvailable() const {
    return availableSeats > 0;
}
