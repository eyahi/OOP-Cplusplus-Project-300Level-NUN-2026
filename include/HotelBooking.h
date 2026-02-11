/**
 * @file HotelBooking.h
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief HotelBooking class derived from Booking
 * 
 * This file defines the HotelBooking class which represents
 * a booking for a specific hotel.
 */

#ifndef HOTELBOOKING_H
#define HOTELBOOKING_H

#include "Booking.h"
#include <string>

/**
 * @class HotelBooking
 * @brief Represents a hotel booking
 * 
 * Inherits from Booking and adds hotel-specific booking attributes
 * such as check-in/out dates, number of rooms and guests.
 */
class HotelBooking : public Booking {
private:
    int hotelId;            ///< ID of the booked hotel
    std::string checkInDate;  ///< Check-in date
    std::string checkOutDate; ///< Check-out date
    int numRooms;           ///< Number of rooms booked
    int numGuests;          ///< Number of guests
    std::string guestName;  ///< Primary guest name

public:
    /**
     * @brief Construct a new HotelBooking object
     * @param bookingId Unique booking ID
     * @param userId User who made the booking
     * @param bookingDate Date of booking
     * @param totalPrice Total price
     * @param hotelId ID of the hotel
     * @param checkInDate Check-in date
     * @param checkOutDate Check-out date
     * @param numRooms Number of rooms
     * @param numGuests Number of guests
     * @param guestName Primary guest name
     */
    HotelBooking(int bookingId, int userId, const std::string& bookingDate,
                 double totalPrice, int hotelId, const std::string& checkInDate,
                 const std::string& checkOutDate, int numRooms, int numGuests,
                 const std::string& guestName);

    // Getters
    int getHotelId() const;
    std::string getCheckInDate() const;
    std::string getCheckOutDate() const;
    int getNumRooms() const;
    int getNumGuests() const;
    std::string getGuestName() const;

    // Overridden virtual functions from Booking
    std::string getDetails() const override;
    std::string getBookingType() const override;
    void displayBooking() const override;
    int getBookableId() const override;
};

#endif // HOTELBOOKING_H
