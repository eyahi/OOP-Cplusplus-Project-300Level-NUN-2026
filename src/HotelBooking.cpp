/**
 * @file HotelBooking.cpp
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Implementation of HotelBooking class
 */

#include "HotelBooking.h"
#include <iostream>
#include <iomanip>
#include <sstream>

HotelBooking::HotelBooking(int bookingId, int userId, const std::string& bookingDate,
                           double totalPrice, int hotelId, const std::string& checkInDate,
                           const std::string& checkOutDate, int numRooms, int numGuests,
                           const std::string& guestName)
    : Booking(bookingId, userId, bookingDate, totalPrice),
      hotelId(hotelId), checkInDate(checkInDate), checkOutDate(checkOutDate),
      numRooms(numRooms), numGuests(numGuests), guestName(guestName) {}

int HotelBooking::getHotelId() const {
    return hotelId;
}

std::string HotelBooking::getCheckInDate() const {
    return checkInDate;
}

std::string HotelBooking::getCheckOutDate() const {
    return checkOutDate;
}

int HotelBooking::getNumRooms() const {
    return numRooms;
}

int HotelBooking::getNumGuests() const {
    return numGuests;
}

std::string HotelBooking::getGuestName() const {
    return guestName;
}

std::string HotelBooking::getDetails() const {
    std::stringstream ss;
    ss << "Hotel Booking #" << bookingId << "\n";
    ss << "  Guest: " << guestName << "\n";
    ss << "  Hotel ID: " << hotelId << "\n";
    ss << "  Check-in: " << checkInDate << "\n";
    ss << "  Check-out: " << checkOutDate << "\n";
    ss << "  Rooms: " << numRooms << ", Guests: " << numGuests << "\n";
    ss << "  Booking Date: " << bookingDate << "\n";
    ss << "  Total Price: $" << std::fixed << std::setprecision(2) << totalPrice << "\n";
    ss << "  Status: " << getStatusString();
    return ss.str();
}

std::string HotelBooking::getBookingType() const {
    return "Hotel";
}

void HotelBooking::displayBooking() const {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    🏨 HOTEL BOOKING                          ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Booking ID: " << std::setw(10) << bookingId 
              << "        Status: " << std::setw(20) << std::left << getStatusString() << "║\n" << std::right;
    std::cout << "║ Booking Date: " << std::setw(47) << std::left << bookingDate << "║\n" << std::right;
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Guest Name: " << std::setw(49) << std::left << guestName << "║\n" << std::right;
    std::cout << "║ Hotel ID: " << std::setw(51) << std::left << hotelId << "║\n" << std::right;
    std::cout << "║ Check-in: " << std::setw(15) << std::left << checkInDate 
              << "      Check-out: " << std::setw(16) << checkOutDate << "  ║\n" << std::right;
    std::cout << "║ Rooms: " << std::setw(5) << numRooms 
              << "                    Guests: " << std::setw(18) << numGuests << "   ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Total Price: $" << std::fixed << std::setprecision(2) 
              << std::setw(46) << std::left << totalPrice << "║\n" << std::right;
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

int HotelBooking::getBookableId() const {
    return hotelId;
}
