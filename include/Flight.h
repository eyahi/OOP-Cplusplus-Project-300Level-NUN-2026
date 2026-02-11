/**
 * @file Flight.h
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Flight class derived from Bookable
 * 
 * This file defines the Flight class which represents a flight
 * in the travel booking system. Demonstrates inheritance from Bookable.
 */

#ifndef FLIGHT_H
#define FLIGHT_H

#include "Bookable.h"
#include <string>

/**
 * @class Flight
 * @brief Represents a flight that can be booked
 * 
 * Inherits from Bookable and adds flight-specific attributes
 * such as airline, origin, destination, and seat availability.
 */
class Flight : public Bookable {
private:
    std::string airline;        ///< Name of the airline
    std::string origin;         ///< Departure city/airport
    std::string destination;    ///< Arrival city/airport
    std::string departureTime;  ///< Departure date and time
    std::string arrivalTime;    ///< Arrival date and time
    int totalSeats;             ///< Total number of seats
    int availableSeats;         ///< Number of seats still available
    std::string flightNumber;   ///< Unique flight number (e.g., "AA123")

public:
    /**
     * @brief Construct a new Flight object
     * @param id Unique flight ID
     * @param name Flight name/description
     * @param price Ticket price
     * @param airline Airline name
     * @param origin Origin city
     * @param destination Destination city
     * @param departureTime Departure date/time string
     * @param arrivalTime Arrival date/time string
     * @param totalSeats Total seat capacity
     * @param flightNumber Flight number code
     */
    Flight(int id, const std::string& name, double price,
           const std::string& airline, const std::string& origin,
           const std::string& destination, const std::string& departureTime,
           const std::string& arrivalTime, int totalSeats,
           const std::string& flightNumber);

    // Getters
    std::string getAirline() const;
    std::string getOrigin() const;
    std::string getDestination() const;
    std::string getDepartureTime() const;
    std::string getArrivalTime() const;
    int getAvailableSeats() const;
    int getTotalSeats() const;
    std::string getFlightNumber() const;

    /**
     * @brief Book seats on this flight
     * @param numSeats Number of seats to book
     * @return true if booking successful, false if not enough seats
     */
    bool bookSeats(int numSeats);

    /**
     * @brief Cancel seat booking
     * @param numSeats Number of seats to release
     */
    void cancelSeats(int numSeats);

    // Overridden virtual functions from Bookable
    void displayInfo() const override;
    std::string getType() const override;
    bool isAvailable() const override;
};

#endif // FLIGHT_H
