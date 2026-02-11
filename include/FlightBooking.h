/**
 * @file FlightBooking.h
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief FlightBooking class derived from Booking
 * 
 * This file defines the FlightBooking class which represents
 * a booking for a specific flight.
 */

#ifndef FLIGHTBOOKING_H
#define FLIGHTBOOKING_H

#include "Booking.h"
#include <string>

/**
 * @class FlightBooking
 * @brief Represents a flight booking
 * 
 * Inherits from Booking and adds flight-specific booking attributes
 * such as number of passengers and seat class.
 */
class FlightBooking : public Booking {
private:
    int flightId;           ///< ID of the booked flight
    int numPassengers;      ///< Number of passengers
    std::string seatClass;  ///< Class of seats (Economy, Business, First)
    std::string passengerName; ///< Primary passenger name

public:
    /**
     * @brief Construct a new FlightBooking object
     * @param bookingId Unique booking ID
     * @param userId User who made the booking
     * @param bookingDate Date of booking
     * @param totalPrice Total price
     * @param flightId ID of the flight
     * @param numPassengers Number of passengers
     * @param seatClass Seat class
     * @param passengerName Primary passenger name
     */
    FlightBooking(int bookingId, int userId, const std::string& bookingDate,
                  double totalPrice, int flightId, int numPassengers,
                  const std::string& seatClass, const std::string& passengerName);

    // Getters
    int getFlightId() const;
    int getNumPassengers() const;
    std::string getSeatClass() const;
    std::string getPassengerName() const;

    // Overridden virtual functions from Booking
    std::string getDetails() const override;
    std::string getBookingType() const override;
    void displayBooking() const override;
    int getBookableId() const override;
};

#endif // FLIGHTBOOKING_H
