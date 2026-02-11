/**
 * @file Hotel.cpp
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Implementation of Hotel class
 */

#include "Hotel.h"
#include <iomanip>
#include <sstream>

Hotel::Hotel(int id, const std::string& name, double price,
             const std::string& location, const std::string& city,
             int starRating, int totalRooms,
             const std::string& checkInTime, const std::string& checkOutTime)
    : Bookable(id, name, price, location),
      starRating(starRating), checkInTime(checkInTime), checkOutTime(checkOutTime),
      totalRooms(totalRooms), availableRooms(totalRooms), city(city) {}

int Hotel::getStarRating() const {
    return starRating;
}

std::string Hotel::getCheckInTime() const {
    return checkInTime;
}

std::string Hotel::getCheckOutTime() const {
    return checkOutTime;
}

int Hotel::getAvailableRooms() const {
    return availableRooms;
}

int Hotel::getTotalRooms() const {
    return totalRooms;
}

std::vector<std::string> Hotel::getAmenities() const {
    return amenities;
}

std::string Hotel::getCity() const {
    return city;
}

void Hotel::addAmenity(const std::string& amenity) {
    amenities.push_back(amenity);
}

bool Hotel::bookRooms(int numRooms) {
    if (numRooms <= 0 || numRooms > availableRooms) {
        return false;
    }
    availableRooms -= numRooms;
    return true;
}

void Hotel::cancelRooms(int numRooms) {
    availableRooms += numRooms;
    if (availableRooms > totalRooms) {
        availableRooms = totalRooms;
    }
}

void Hotel::displayInfo() const {
    // Create star rating display
    std::string stars = "";
    for (int i = 0; i < starRating; ++i) {
        stars += "★";
    }
    for (int i = starRating; i < 5; ++i) {
        stars += "☆";
    }

    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                      🏨 HOTEL DETAILS                        ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Hotel ID: " << std::setw(8) << id 
              << "          Rating: " << stars << "          ║\n";
    std::cout << "║ Name: " << std::setw(54) << std::left << name << "║\n" << std::right;
    std::cout << "║ City: " << std::setw(54) << std::left << city << "║\n" << std::right;
    std::cout << "║ Address: " << std::setw(51) << std::left << location << "║\n" << std::right;
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ Check-in: " << std::setw(10) << checkInTime 
              << "            Check-out: " << std::setw(15) << checkOutTime << "  ║\n";
    std::cout << "║ Price/Night: $" << std::fixed << std::setprecision(2) << std::setw(8) << price
              << "       Available Rooms: " << std::setw(4) << availableRooms 
              << "/" << std::setw(4) << totalRooms << "   ║\n";
    
    if (!amenities.empty()) {
        std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
        std::cout << "║ Amenities:                                                   ║\n";
        std::string amenityLine = "  ";
        for (size_t i = 0; i < amenities.size(); ++i) {
            if (amenityLine.length() + amenities[i].length() + 4 > 60) {
                std::cout << "║" << std::setw(62) << std::left << amenityLine << "║\n";
                amenityLine = "  ";
            }
            amenityLine += "• " + amenities[i] + "  ";
        }
        if (amenityLine.length() > 2) {
            std::cout << "║" << std::setw(62) << std::left << amenityLine << "║\n";
        }
    }
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

std::string Hotel::getType() const {
    return "Hotel";
}

bool Hotel::isAvailable() const {
    return availableRooms > 0;
}
