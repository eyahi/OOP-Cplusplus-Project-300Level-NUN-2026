/**
 * @file FlightBooking.cpp
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Implementation of FlightBooking class
 */

#include "FlightBooking.h"
#include <iostream>
#include <iomanip>
#include <sstream>

FlightBooking::FlightBooking(int bookingId, int userId, const std::string& bookingDate,
                             double totalPrice, int flightId, int numPassengers,
                             const std::string& seatClass, const std::string& passengerName)
    : Booking(bookingId, userId, bookingDate, totalPrice),
      flightId(flightId), numPassengers(numPassengers),
      seatClass(seatClass), passengerName(passengerName) {}

int FlightBooking::getFlightId() const {
    return flightId;
}

int FlightBooking::getNumPassengers() const {
    return numPassengers;
}

std::string FlightBooking::getSeatClass() const {
    return seatClass;
}

std::string FlightBooking::getPassengerName() const {
    return passengerName;
}

std::string FlightBooking::getDetails() const {
    std::stringstream ss;
    ss << "Flight Booking #" << bookingId << "\n";
    ss << "  Passenger: " << passengerName << "\n";
    ss << "  Flight ID: " << flightId << "\n";
    ss << "  Passengers: " << numPassengers << "\n";
    ss << "  Class: " << seatClass << "\n";
    ss << "  Booking Date: " << bookingDate << "\n";
    ss << "  Total Price: $" << std::fixed << std::setprecision(2) << totalPrice << "\n";
    ss << "  Status: " << getStatusString();
    return ss.str();
}

std::string FlightBooking::getBookingType() const {
    return "Flight";
}

void FlightBooking::displayBooking() const {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                   ✈️  FLIGHT BOOKING                          ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Booking ID: " << std::setw(10) << bookingId 
              << "        Status: " << std::setw(20) << std::left << getStatusString() << "║\n" << std::right;
    std::cout << "║ Booking Date: " << std::setw(47) << std::left << bookingDate << "║\n" << std::right;
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Passenger: " << std::setw(50) << std::left << passengerName << "║\n" << std::right;
    std::cout << "║ Flight ID: " << std::setw(10) << flightId 
              << "        Passengers: " << std::setw(15) << numPassengers << "   ║\n";
    std::cout << "║ Seat Class: " << std::setw(49) << std::left << seatClass << "║\n" << std::right;
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Total Price: $" << std::fixed << std::setprecision(2) 
              << std::setw(46) << std::left << totalPrice << "║\n" << std::right;
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

int FlightBooking::getBookableId() const {
    return flightId;
}
