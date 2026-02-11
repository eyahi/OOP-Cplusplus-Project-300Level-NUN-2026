/**
 * @file Booking.h
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Abstract base class for all booking types
 * 
 * This file defines the Booking abstract class which serves as the
 * base for FlightBooking and HotelBooking classes.
 */

#ifndef BOOKING_H
#define BOOKING_H

#include <string>

/**
 * @brief Enumeration for booking status
 */
enum class BookingStatus {
    PENDING,
    CONFIRMED,
    CANCELLED,
    COMPLETED
};

/**
 * @class Booking
 * @brief Abstract base class for all bookings
 * 
 * Provides common interface and attributes for all booking types.
 * Uses pure virtual functions for polymorphic behavior.
 */
class Booking {
protected:
    int bookingId;          ///< Unique booking identifier
    int userId;             ///< ID of user who made the booking
    std::string bookingDate;///< Date when booking was made
    double totalPrice;      ///< Total price of the booking
    BookingStatus status;   ///< Current status of the booking

public:
    /**
     * @brief Construct a new Booking object
     * @param bookingId Unique booking ID
     * @param userId User ID who made the booking
     * @param bookingDate Date of booking
     * @param totalPrice Total price
     */
    Booking(int bookingId, int userId, const std::string& bookingDate, double totalPrice);

    /**
     * @brief Virtual destructor
     */
    virtual ~Booking() = default;

    // Getters
    int getBookingId() const;
    int getUserId() const;
    std::string getBookingDate() const;
    double getTotalPrice() const;
    BookingStatus getStatus() const;
    std::string getStatusString() const;

    /**
     * @brief Confirm the booking
     */
    void confirm();

    /**
     * @brief Cancel the booking
     */
    void cancel();

    /**
     * @brief Get detailed booking information
     * @return String with all booking details
     */
    virtual std::string getDetails() const = 0;

    /**
     * @brief Get the type of booking
     * @return String representing booking type
     */
    virtual std::string getBookingType() const = 0;

    /**
     * @brief Display booking information
     */
    virtual void displayBooking() const = 0;

    /**
     * @brief Get the associated bookable item ID
     * @return ID of the flight or hotel
     */
    virtual int getBookableId() const = 0;
};

#endif // BOOKING_H
