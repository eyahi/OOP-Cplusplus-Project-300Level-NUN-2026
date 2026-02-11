/**
 * @file BookingSystem.h
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Main booking system class managing all operations
 * 
 * This file defines the BookingSystem class which is the central
 * controller for the travel booking application, managing flights,
 * hotels, users, and bookings.
 */

#ifndef BOOKINGSYSTEM_H
#define BOOKINGSYSTEM_H

#include "Flight.h"
#include "Hotel.h"
#include "User.h"
#include "Booking.h"
#include "FlightBooking.h"
#include "HotelBooking.h"
#include <vector>
#include <memory>
#include <string>

/**
 * @class BookingSystem
 * @brief Central system managing all travel booking operations
 * 
 * Manages the inventory of flights and hotels, user accounts,
 * and all bookings. Handles file I/O for data persistence.
 */
class BookingSystem {
private:
    std::vector<std::unique_ptr<Flight>> flights;   ///< All available flights
    std::vector<std::unique_ptr<Hotel>> hotels;     ///< All available hotels
    std::vector<User> users;                         ///< Registered users
    std::vector<std::unique_ptr<Booking>> bookings; ///< All bookings
    User* currentUser;                               ///< Currently logged in user
    int nextBookingId;                               ///< Counter for booking IDs
    int nextUserId;                                  ///< Counter for user IDs
    std::string lastBookedDestination;               ///< Destination from last flight booking

    // File paths
    const std::string FLIGHTS_FILE = "data/flights.txt";
    const std::string HOTELS_FILE = "data/hotels.txt";
    const std::string USERS_FILE = "data/users.txt";
    const std::string BOOKINGS_FILE = "data/bookings.txt";

    /**
     * @brief Generate current date string
     * @return Current date in YYYY-MM-DD format
     */
    std::string getCurrentDate() const;

public:
    /**
     * @brief Construct a new BookingSystem object
     */
    BookingSystem();

    /**
     * @brief Destructor - saves all data before exit
     */
    ~BookingSystem();

    // ===== Data Loading/Saving =====
    
    /**
     * @brief Load all data from files
     */
    void loadAllData();

    /**
     * @brief Save all data to files
     */
    void saveAllData();

    void loadFlights();
    void loadHotels();
    void loadUsers();
    void loadBookings();

    void saveFlights();
    void saveHotels();
    void saveUsers();
    void saveBookings();

    // ===== User Management =====
    
    /**
     * @brief Register a new user
     * @param username Username
     * @param password Password
     * @param email Email address
     * @param fullName Full name
     * @param phone Phone number
     * @return true if registration successful
     */
    bool registerUser(const std::string& username, const std::string& password,
                      const std::string& email, const std::string& fullName,
                      const std::string& phone);

    /**
     * @brief Log in a user
     * @param username Username
     * @param password Password
     * @return true if login successful
     */
    bool login(const std::string& username, const std::string& password);

    /**
     * @brief Log out the current user
     */
    void logout();

    /**
     * @brief Check if a user is logged in
     * @return true if logged in
     */
    bool isLoggedIn() const;

    /**
     * @brief Get the current user
     * @return Pointer to current user or nullptr
     */
    User* getCurrentUser() const;

    // ===== Search Functions =====
    
    /**
     * @brief Search for flights by origin and destination
     * @param origin Origin city
     * @param destination Destination city
     * @return Vector of matching flights
     */
    std::vector<Flight*> searchFlights(const std::string& origin, 
                                        const std::string& destination) const;

    /**
     * @brief Search for hotels by city
     * @param city City name
     * @return Vector of matching hotels
     */
    std::vector<Hotel*> searchHotels(const std::string& city) const;

    /**
     * @brief Search hotels by star rating
     * @param city City name
     * @param minStars Minimum star rating
     * @return Vector of matching hotels
     */
    std::vector<Hotel*> searchHotelsByRating(const std::string& city, 
                                              int minStars) const;

    /**
     * @brief Get hotels in the last booked flight destination
     * @return Vector of hotels in destination city
     */
    std::vector<Hotel*> getHotelsInDestination() const;

    /**
     * @brief Get the last booked destination
     * @return Destination city name
     */
    std::string getLastBookedDestination() const;

    // ===== Booking Functions =====
    
    /**
     * @brief Book a flight
     * @param flightId Flight ID
     * @param numPassengers Number of passengers
     * @param seatClass Seat class
     * @param passengerName Passenger name
     * @return true if booking successful
     */
    bool bookFlight(int flightId, int numPassengers, 
                    const std::string& seatClass, const std::string& passengerName);

    /**
     * @brief Book a hotel
     * @param hotelId Hotel ID
     * @param checkIn Check-in date
     * @param checkOut Check-out date
     * @param numRooms Number of rooms
     * @param numGuests Number of guests
     * @param guestName Guest name
     * @return true if booking successful
     */
    bool bookHotel(int hotelId, const std::string& checkIn, 
                   const std::string& checkOut, int numRooms, 
                   int numGuests, const std::string& guestName);

    /**
     * @brief Cancel a booking
     * @param bookingId Booking ID to cancel
     * @return true if cancellation successful
     */
    bool cancelBooking(int bookingId);

    /**
     * @brief Get all bookings for current user
     * @return Vector of user's bookings
     */
    std::vector<Booking*> getUserBookings() const;

    /**
     * @brief Get all bookings
     * @return Vector of all bookings
     */
    std::vector<Booking*> getAllBookings() const;

    // ===== Display Functions =====
    
    /**
     * @brief Display all available flights
     */
    void displayAllFlights() const;

    /**
     * @brief Display all available hotels
     */
    void displayAllHotels() const;

    /**
     * @brief Find flight by ID
     * @param flightId Flight ID
     * @return Pointer to flight or nullptr
     */
    Flight* findFlightById(int flightId) const;

    /**
     * @brief Find hotel by ID
     * @param hotelId Hotel ID
     * @return Pointer to hotel or nullptr
     */
    Hotel* findHotelById(int hotelId) const;
};

#endif // BOOKINGSYSTEM_H
