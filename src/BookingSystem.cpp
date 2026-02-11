/**
 * @file BookingSystem.cpp
 * @author [Your Name]
 * @date 2026-01-21
 * @brief Implementation of BookingSystem class
 * 
 * Main system class that manages all travel booking operations,
 * including flights, hotels, users, and bookings with file I/O.
 */

#include "BookingSystem.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>

BookingSystem::BookingSystem() 
    : currentUser(nullptr), nextBookingId(1), nextUserId(1), lastBookedDestination("") {
    loadAllData();
}

BookingSystem::~BookingSystem() {
    saveAllData();
}

std::string BookingSystem::getCurrentDate() const {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << (localTime->tm_year + 1900) << "-"
       << std::setw(2) << (localTime->tm_mon + 1) << "-"
       << std::setw(2) << localTime->tm_mday;
    return ss.str();
}

// ===== Data Loading Functions =====

void BookingSystem::loadAllData() {
    loadFlights();
    loadHotels();
    loadUsers();
    loadBookings();
}

void BookingSystem::loadFlights() {
    std::ifstream file(FLIGHTS_FILE);
    if (!file.is_open()) {
        std::cout << "[INFO] No flights data file found. Starting fresh.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::stringstream ss(line);
        int id, totalSeats;
        double price;
        std::string name, airline, origin, destination, departure, arrival, flightNum;
        
        std::getline(ss, line, '|'); id = std::stoi(line);
        std::getline(ss, name, '|');
        std::getline(ss, line, '|'); price = std::stod(line);
        std::getline(ss, airline, '|');
        std::getline(ss, origin, '|');
        std::getline(ss, destination, '|');
        std::getline(ss, departure, '|');
        std::getline(ss, arrival, '|');
        std::getline(ss, line, '|'); totalSeats = std::stoi(line);
        std::getline(ss, flightNum, '|');

        flights.push_back(std::make_unique<Flight>(
            id, name, price, airline, origin, destination, 
            departure, arrival, totalSeats, flightNum
        ));
    }
    file.close();
}

void BookingSystem::loadHotels() {
    std::ifstream file(HOTELS_FILE);
    if (!file.is_open()) {
        std::cout << "[INFO] No hotels data file found. Starting fresh.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::stringstream ss(line);
        int id, starRating, totalRooms;
        double price;
        std::string name, location, city, checkIn, checkOut, amenitiesStr;
        
        std::getline(ss, line, '|'); id = std::stoi(line);
        std::getline(ss, name, '|');
        std::getline(ss, line, '|'); price = std::stod(line);
        std::getline(ss, location, '|');
        std::getline(ss, city, '|');
        std::getline(ss, line, '|'); starRating = std::stoi(line);
        std::getline(ss, line, '|'); totalRooms = std::stoi(line);
        std::getline(ss, checkIn, '|');
        std::getline(ss, checkOut, '|');
        std::getline(ss, amenitiesStr);

        auto hotel = std::make_unique<Hotel>(
            id, name, price, location, city, starRating, totalRooms, checkIn, checkOut
        );

        // Parse amenities (comma-separated)
        if (!amenitiesStr.empty()) {
            std::stringstream amenitySS(amenitiesStr);
            std::string amenity;
            while (std::getline(amenitySS, amenity, ',')) {
                if (!amenity.empty()) {
                    hotel->addAmenity(amenity);
                }
            }
        }

        hotels.push_back(std::move(hotel));
    }
    file.close();
}

void BookingSystem::loadUsers() {
    std::ifstream file(USERS_FILE);
    if (!file.is_open()) {
        std::cout << "[INFO] No users data file found. Starting fresh.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::stringstream ss(line);
        int id;
        std::string username, password, email, fullName, phone;
        
        std::getline(ss, line, '|'); id = std::stoi(line);
        std::getline(ss, username, '|');
        std::getline(ss, password, '|');
        std::getline(ss, email, '|');
        std::getline(ss, fullName, '|');
        std::getline(ss, phone);

        users.emplace_back(id, username, password, email, fullName, phone);
        if (id >= nextUserId) {
            nextUserId = id + 1;
        }
    }
    file.close();
}

void BookingSystem::loadBookings() {
    std::ifstream file(BOOKINGS_FILE);
    if (!file.is_open()) {
        std::cout << "[INFO] No bookings data file found. Starting fresh.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::stringstream ss(line);
        std::string type;
        std::getline(ss, type, '|');

        if (type == "FLIGHT") {
            int bookingId, userId, flightId, numPass;
            double price;
            std::string date, seatClass, passName;
            
            std::getline(ss, line, '|'); bookingId = std::stoi(line);
            std::getline(ss, line, '|'); userId = std::stoi(line);
            std::getline(ss, date, '|');
            std::getline(ss, line, '|'); price = std::stod(line);
            std::getline(ss, line, '|'); flightId = std::stoi(line);
            std::getline(ss, line, '|'); numPass = std::stoi(line);
            std::getline(ss, seatClass, '|');
            std::getline(ss, passName);

            bookings.push_back(std::make_unique<FlightBooking>(
                bookingId, userId, date, price, flightId, numPass, seatClass, passName
            ));
            
            if (bookingId >= nextBookingId) {
                nextBookingId = bookingId + 1;
            }
        } else if (type == "HOTEL") {
            int bookingId, userId, hotelId, numRooms, numGuests;
            double price;
            std::string date, checkIn, checkOut, guestName;
            
            std::getline(ss, line, '|'); bookingId = std::stoi(line);
            std::getline(ss, line, '|'); userId = std::stoi(line);
            std::getline(ss, date, '|');
            std::getline(ss, line, '|'); price = std::stod(line);
            std::getline(ss, line, '|'); hotelId = std::stoi(line);
            std::getline(ss, checkIn, '|');
            std::getline(ss, checkOut, '|');
            std::getline(ss, line, '|'); numRooms = std::stoi(line);
            std::getline(ss, line, '|'); numGuests = std::stoi(line);
            std::getline(ss, guestName);

            bookings.push_back(std::make_unique<HotelBooking>(
                bookingId, userId, date, price, hotelId, checkIn, checkOut, 
                numRooms, numGuests, guestName
            ));
            
            if (bookingId >= nextBookingId) {
                nextBookingId = bookingId + 1;
            }
        }
    }
    file.close();
}

// ===== Data Saving Functions =====

void BookingSystem::saveAllData() {
    saveFlights();
    saveHotels();
    saveUsers();
    saveBookings();
}

void BookingSystem::saveFlights() {
    std::ofstream file(FLIGHTS_FILE);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not save flights data.\n";
        return;
    }

    file << "# Flight data format: id|name|price|airline|origin|destination|departure|arrival|availableSeats|flightNum\n";
    for (const auto& flight : flights) {
        file << flight->getId() << "|"
             << flight->getName() << "|"
             << flight->getPrice() << "|"
             << flight->getAirline() << "|"
             << flight->getOrigin() << "|"
             << flight->getDestination() << "|"
             << flight->getDepartureTime() << "|"
             << flight->getArrivalTime() << "|"
             << flight->getAvailableSeats() << "|"
             << flight->getFlightNumber() << "\n";
    }
    file.close();
}

void BookingSystem::saveHotels() {
    std::ofstream file(HOTELS_FILE);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not save hotels data.\n";
        return;
    }

    file << "# Hotel data format: id|name|price|location|city|stars|availableRooms|checkIn|checkOut|amenities\n";
    for (const auto& hotel : hotels) {
        file << hotel->getId() << "|"
             << hotel->getName() << "|"
             << hotel->getPrice() << "|"
             << hotel->getLocation() << "|"
             << hotel->getCity() << "|"
             << hotel->getStarRating() << "|"
             << hotel->getAvailableRooms() << "|"
             << hotel->getCheckInTime() << "|"
             << hotel->getCheckOutTime() << "|";
        
        auto amenities = hotel->getAmenities();
        for (size_t i = 0; i < amenities.size(); ++i) {
            file << amenities[i];
            if (i < amenities.size() - 1) file << ",";
        }
        file << "\n";
    }
    file.close();
}

void BookingSystem::saveUsers() {
    std::ofstream file(USERS_FILE);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not save users data.\n";
        return;
    }

    file << "# User data format: id|username|password|email|fullName|phone\n";
    for (const auto& user : users) {
        file << user.getUserId() << "|"
             << user.getUsername() << "|"
             << user.getPassword() << "|"
             << user.getEmail() << "|"
             << user.getFullName() << "|"
             << user.getPhone() << "\n";
    }
    file.close();
}

void BookingSystem::saveBookings() {
    std::ofstream file(BOOKINGS_FILE);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not save bookings data.\n";
        return;
    }

    file << "# Booking data format: TYPE|bookingId|userId|date|price|...specific fields...\n";
    for (const auto& booking : bookings) {
        if (booking->getBookingType() == "Flight") {
            auto* fb = dynamic_cast<FlightBooking*>(booking.get());
            if (fb) {
                file << "FLIGHT|"
                     << fb->getBookingId() << "|"
                     << fb->getUserId() << "|"
                     << fb->getBookingDate() << "|"
                     << fb->getTotalPrice() << "|"
                     << fb->getFlightId() << "|"
                     << fb->getNumPassengers() << "|"
                     << fb->getSeatClass() << "|"
                     << fb->getPassengerName() << "\n";
            }
        } else if (booking->getBookingType() == "Hotel") {
            auto* hb = dynamic_cast<HotelBooking*>(booking.get());
            if (hb) {
                file << "HOTEL|"
                     << hb->getBookingId() << "|"
                     << hb->getUserId() << "|"
                     << hb->getBookingDate() << "|"
                     << hb->getTotalPrice() << "|"
                     << hb->getHotelId() << "|"
                     << hb->getCheckInDate() << "|"
                     << hb->getCheckOutDate() << "|"
                     << hb->getNumRooms() << "|"
                     << hb->getNumGuests() << "|"
                     << hb->getGuestName() << "\n";
            }
        }
    }
    file.close();
}

// ===== User Management =====

bool BookingSystem::registerUser(const std::string& username, const std::string& password,
                                  const std::string& email, const std::string& fullName,
                                  const std::string& phone) {
    // Check if username already exists
    for (const auto& user : users) {
        if (user.getUsername() == username) {
            return false;
        }
    }

    users.emplace_back(nextUserId++, username, password, email, fullName, phone);
    saveUsers();
    return true;
}

bool BookingSystem::login(const std::string& username, const std::string& password) {
    for (auto& user : users) {
        if (user.getUsername() == username && user.authenticate(password)) {
            currentUser = &user;
            return true;
        }
    }
    return false;
}

void BookingSystem::logout() {
    currentUser = nullptr;
    lastBookedDestination = "";
}

bool BookingSystem::isLoggedIn() const {
    return currentUser != nullptr;
}

User* BookingSystem::getCurrentUser() const {
    return currentUser;
}

// ===== Search Functions =====

std::vector<Flight*> BookingSystem::searchFlights(const std::string& origin, 
                                                   const std::string& destination) const {
    std::vector<Flight*> results;
    
    // Convert search terms to lowercase for case-insensitive search
    std::string lowerOrigin = origin;
    std::string lowerDest = destination;
    std::transform(lowerOrigin.begin(), lowerOrigin.end(), lowerOrigin.begin(), ::tolower);
    std::transform(lowerDest.begin(), lowerDest.end(), lowerDest.begin(), ::tolower);

    for (const auto& flight : flights) {
        std::string flightOrigin = flight->getOrigin();
        std::string flightDest = flight->getDestination();
        std::transform(flightOrigin.begin(), flightOrigin.end(), flightOrigin.begin(), ::tolower);
        std::transform(flightDest.begin(), flightDest.end(), flightDest.begin(), ::tolower);

        bool matchOrigin = origin.empty() || flightOrigin.find(lowerOrigin) != std::string::npos;
        bool matchDest = destination.empty() || flightDest.find(lowerDest) != std::string::npos;

        if (matchOrigin && matchDest && flight->isAvailable()) {
            results.push_back(flight.get());
        }
    }
    return results;
}

std::vector<Hotel*> BookingSystem::searchHotels(const std::string& city) const {
    std::vector<Hotel*> results;
    
    std::string lowerCity = city;
    std::transform(lowerCity.begin(), lowerCity.end(), lowerCity.begin(), ::tolower);

    for (const auto& hotel : hotels) {
        std::string hotelCity = hotel->getCity();
        std::transform(hotelCity.begin(), hotelCity.end(), hotelCity.begin(), ::tolower);

        if ((city.empty() || hotelCity.find(lowerCity) != std::string::npos) && hotel->isAvailable()) {
            results.push_back(hotel.get());
        }
    }
    return results;
}

std::vector<Hotel*> BookingSystem::searchHotelsByRating(const std::string& city, int minStars) const {
    std::vector<Hotel*> results;
    auto cityHotels = searchHotels(city);
    
    for (auto* hotel : cityHotels) {
        if (hotel->getStarRating() >= minStars) {
            results.push_back(hotel);
        }
    }
    return results;
}

std::vector<Hotel*> BookingSystem::getHotelsInDestination() const {
    if (lastBookedDestination.empty()) {
        return {};
    }
    return searchHotels(lastBookedDestination);
}

std::string BookingSystem::getLastBookedDestination() const {
    return lastBookedDestination;
}

// ===== Booking Functions =====

bool BookingSystem::bookFlight(int flightId, int numPassengers, 
                                const std::string& seatClass, const std::string& passengerName) {
    if (!isLoggedIn()) return false;

    Flight* flight = findFlightById(flightId);
    if (!flight || !flight->bookSeats(numPassengers)) {
        return false;
    }

    // Calculate price based on seat class
    double priceMultiplier = 1.0;
    if (seatClass == "Business") priceMultiplier = 2.0;
    else if (seatClass == "First") priceMultiplier = 3.0;

    double totalPrice = flight->getPrice() * numPassengers * priceMultiplier;

    bookings.push_back(std::make_unique<FlightBooking>(
        nextBookingId++, currentUser->getUserId(), getCurrentDate(),
        totalPrice, flightId, numPassengers, seatClass, passengerName
    ));

    // Store destination for hotel recommendation
    lastBookedDestination = flight->getDestination();

    bookings.back()->confirm();
    saveBookings();
    saveFlights();  // Save updated seat counts
    return true;
}

bool BookingSystem::bookHotel(int hotelId, const std::string& checkIn, 
                               const std::string& checkOut, int numRooms, 
                               int numGuests, const std::string& guestName) {
    if (!isLoggedIn()) return false;

    Hotel* hotel = findHotelById(hotelId);
    if (!hotel || !hotel->bookRooms(numRooms)) {
        return false;
    }

    // Simple calculation: assume 1 night per booking (could be enhanced)
    double totalPrice = hotel->getPrice() * numRooms;

    bookings.push_back(std::make_unique<HotelBooking>(
        nextBookingId++, currentUser->getUserId(), getCurrentDate(),
        totalPrice, hotelId, checkIn, checkOut, numRooms, numGuests, guestName
    ));

    bookings.back()->confirm();
    saveBookings();
    saveHotels();  // Save updated room counts
    return true;
}

bool BookingSystem::cancelBooking(int bookingId) {
    if (!isLoggedIn()) return false;

    for (auto& booking : bookings) {
        if (booking->getBookingId() == bookingId && 
            booking->getUserId() == currentUser->getUserId()) {
            
            // Restore availability
            if (booking->getBookingType() == "Flight") {
                auto* fb = dynamic_cast<FlightBooking*>(booking.get());
                if (fb) {
                    Flight* flight = findFlightById(fb->getFlightId());
                    if (flight) {
                        flight->cancelSeats(fb->getNumPassengers());
                    }
                }
            } else if (booking->getBookingType() == "Hotel") {
                auto* hb = dynamic_cast<HotelBooking*>(booking.get());
                if (hb) {
                    Hotel* hotel = findHotelById(hb->getHotelId());
                    if (hotel) {
                        hotel->cancelRooms(hb->getNumRooms());
                    }
                }
            }

            booking->cancel();
            saveBookings();
            return true;
        }
    }
    return false;
}

std::vector<Booking*> BookingSystem::getUserBookings() const {
    std::vector<Booking*> results;
    if (!isLoggedIn()) return results;

    for (const auto& booking : bookings) {
        if (booking->getUserId() == currentUser->getUserId()) {
            results.push_back(booking.get());
        }
    }
    return results;
}

std::vector<Booking*> BookingSystem::getAllBookings() const {
    std::vector<Booking*> results;
    for (const auto& booking : bookings) {
        results.push_back(booking.get());
    }
    return results;
}

// ===== Display Functions =====

void BookingSystem::displayAllFlights() const {
    if (flights.empty()) {
        std::cout << "\n[INFO] No flights available.\n";
        return;
    }

    std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
    std::cout << "                        AVAILABLE FLIGHTS\n";
    std::cout << "═══════════════════════════════════════════════════════════════════\n";

    for (const auto& flight : flights) {
        if (flight->isAvailable()) {
            flight->displayInfo();
        }
    }
}

void BookingSystem::displayAllHotels() const {
    if (hotels.empty()) {
        std::cout << "\n[INFO] No hotels available.\n";
        return;
    }

    std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
    std::cout << "                        AVAILABLE HOTELS\n";
    std::cout << "═══════════════════════════════════════════════════════════════════\n";

    for (const auto& hotel : hotels) {
        if (hotel->isAvailable()) {
            hotel->displayInfo();
        }
    }
}

Flight* BookingSystem::findFlightById(int flightId) const {
    for (const auto& flight : flights) {
        if (flight->getId() == flightId) {
            return flight.get();
        }
    }
    return nullptr;
}

Hotel* BookingSystem::findHotelById(int hotelId) const {
    for (const auto& hotel : hotels) {
        if (hotel->getId() == hotelId) {
            return hotel.get();
        }
    }
    return nullptr;
}
