/**
 * @file Hotel.h
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Hotel class derived from Bookable
 * 
 * This file defines the Hotel class which represents a hotel
 * in the travel booking system. Demonstrates inheritance from Bookable.
 */

#ifndef HOTEL_H
#define HOTEL_H

#include "Bookable.h"
#include <string>
#include <vector>

/**
 * @class Hotel
 * @brief Represents a hotel that can be booked
 * 
 * Inherits from Bookable and adds hotel-specific attributes
 * such as star rating, amenities, and room availability.
 */
class Hotel : public Bookable {
private:
    int starRating;                     ///< Hotel star rating (1-5)
    std::string checkInTime;            ///< Standard check-in time
    std::string checkOutTime;           ///< Standard check-out time
    int totalRooms;                     ///< Total number of rooms
    int availableRooms;                 ///< Rooms currently available
    std::vector<std::string> amenities; ///< List of hotel amenities
    std::string city;                   ///< City where hotel is located

public:
    /**
     * @brief Construct a new Hotel object
     * @param id Unique hotel ID
     * @param name Hotel name
     * @param price Price per night
     * @param location Full address
     * @param city City name (for filtering)
     * @param starRating Star rating (1-5)
     * @param totalRooms Total room count
     * @param checkInTime Check-in time
     * @param checkOutTime Check-out time
     */
    Hotel(int id, const std::string& name, double price,
          const std::string& location, const std::string& city,
          int starRating, int totalRooms,
          const std::string& checkInTime, const std::string& checkOutTime);

    // Getters
    int getStarRating() const;
    std::string getCheckInTime() const;
    std::string getCheckOutTime() const;
    int getAvailableRooms() const;
    int getTotalRooms() const;
    std::vector<std::string> getAmenities() const;
    std::string getCity() const;

    /**
     * @brief Add an amenity to the hotel
     * @param amenity Name of the amenity
     */
    void addAmenity(const std::string& amenity);

    /**
     * @brief Book rooms at this hotel
     * @param numRooms Number of rooms to book
     * @return true if booking successful, false if not enough rooms
     */
    bool bookRooms(int numRooms);

    /**
     * @brief Cancel room booking
     * @param numRooms Number of rooms to release
     */
    void cancelRooms(int numRooms);

    // Overridden virtual functions from Bookable
    void displayInfo() const override;
    std::string getType() const override;
    bool isAvailable() const override;
};

#endif // HOTEL_H
