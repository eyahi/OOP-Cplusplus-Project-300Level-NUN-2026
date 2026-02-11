# TravelBook - Premium Travel Booking System

A professional C++ travel booking application featuring both a **Console Interface** and a **Premium GUI** built with the Nana C++ Library. Demonstrates advanced Object-Oriented Programming concepts with a focus on clean architecture and code reusability.

---

## Project Information

| Field | Value |
|-------|-------|
| **Course** | Object-Oriented Programming with C++ |
| **Author** | Muhammad Tasiu-20231723, Muhammad Auwal-20231925, Muhammad Lele-20231940 |
| **Date** | January 2026 |
| **Language** | C++17 |
| **GUI Framework** | Nana C++ Library |
| **Build System** | CMake / Make |

---

## Features

### Core Functionality
-  **Flight Booking**: Search and book flights by origin/destination with seat class selection
-  **Hotel Booking**: Search and book hotels by city and star rating
-  **Smart Recommendations**: After booking a flight, automatically suggests hotels at your destination
-  **User Management**: Register, login, and manage your profile
-  **Booking Management**: View all bookings with the ability to cancel
-  **Data Persistence**: All data saved to text files automatically
-  **Premium GUI**: Dark-themed modern interface with professional styling

### User Interface Options

| Interface | Description |
|-----------|-------------|
| **Console (TravelBookingConsole)** | Full-featured text-based menu system |
| **GUI (TravelBookingGUI)** | Premium dark-themed graphical interface with Nana |

---

## OOP Concepts Demonstrated

### Class Hierarchy

```
Bookable (Abstract Base)
├── Flight
└── Hotel

Booking (Abstract Base)
├── FlightBooking
└── HotelBooking

User
BookingSystem (Controller/Manager)
Menu (Console UI)
MainWindow (GUI)
```

### Concept Implementation Table

| OOP Concept | Where Implemented | Example |
|-------------|-------------------|---------|
| **Inheritance** | `Flight : Bookable`, `Hotel : Bookable` | Derived classes extend abstract base |
| **Polymorphism** | `displayInfo()`, `isAvailable()`, `getType()` | Virtual functions with runtime dispatch |
| **Encapsulation** | All classes | Private members with public getters/setters |
| **Abstraction** | `Bookable`, `Booking` classes | Pure virtual functions (`= 0`) |
| **Composition** | `BookingSystem` | Contains vectors of bookings, flights, hotels, users |
| **Smart Pointers** | `BookingSystem` | `std::unique_ptr` for automatic memory management |
| **STL Containers** | Throughout | `std::vector`, `std::string`, algorithms |
| **RAII** | File handling | Resources released in destructors |
| **Const Correctness** | All getter methods | `const` member functions |

---

## Project Architecture

### Directory Structure

```
OOP_CPP_Project/
├── include/                    # Header files (.h)
│   ├── Bookable.h             # Abstract base class for bookable items
│   ├── Flight.h               # Flight class - inherits Bookable
│   ├── Hotel.h                # Hotel class - inherits Bookable
│   ├── User.h                 # User account management
│   ├── Booking.h              # Abstract base class for bookings
│   ├── FlightBooking.h        # Flight booking - inherits Booking
│   ├── HotelBooking.h         # Hotel booking - inherits Booking
│   ├── BookingSystem.h        # Central system controller
│   ├── Menu.h                 # Console menu interface
│   └── gui/                   # GUI components
│       ├── MainWindow.h       # Main application window
│       ├── FlightSearchPanel.h
│       ├── HotelSearchPanel.h
│       └── Styles.h           # Premium styling constants
├── src/                        # Implementation files (.cpp)
│   ├── Bookable.cpp
│   ├── Flight.cpp
│   ├── Hotel.cpp
│   ├── User.cpp
│   ├── Booking.cpp
│   ├── FlightBooking.cpp
│   ├── HotelBooking.cpp
│   ├── BookingSystem.cpp
│   ├── Menu.cpp
│   ├── main.cpp               # Console entry point
│   └── gui/
│       ├── MainWindow.cpp
│       └── gui_main.cpp       # GUI entry point
├── lib/
│   └── nana/                  # Nana GUI library (submodule)
├── data/                       # Persistent data files
│   ├── flights.txt
│   ├── hotels.txt
│   ├── users.txt
│   └── bookings.txt
├── build/                      # Build output (generated)
├── CMakeLists.txt             # CMake configuration
├── Makefile                   # Make configuration
└── README.md                  # This documentation
```

### Class Descriptions

#### Abstract Base Classes

| Class | Purpose | Key Virtual Methods |
|-------|---------|---------------------|
| `Bookable` | Base for items that can be booked | `displayInfo()`, `isAvailable()`, `getType()` |
| `Booking` | Base for booking records | `getBookingType()`, `getTotalPrice()`, `getDetails()` |

#### Concrete Classes

| Class | Description | Key Methods |
|-------|-------------|-------------|
| `Flight` | Represents a flight | `bookSeats()`, `cancelSeats()`, `getDestination()` |
| `Hotel` | Represents a hotel | `bookRooms()`, `cancelRooms()`, `getAmenities()` |
| `FlightBooking` | A completed flight reservation | `getFlightId()`, `getSeatClass()`, `getNumPassengers()` |
| `HotelBooking` | A completed hotel reservation | `getHotelId()`, `getCheckInDate()`, `getNumRooms()` |
| `User` | User account | `authenticate()`, `getUsername()`, `getEmail()` |
| `BookingSystem` | Central controller | `bookFlight()`, `bookHotel()`, `searchFlights()`, `searchHotels()` |

---

## Building the Project

### Prerequisites

- **C++ Compiler**: g++ 10+ or MSVC 2019+ with C++17 support
- **CMake**: Version 3.15+ (for GUI version)
- **Git**: For cloning Nana library

### Option 1: CMake Build (Recommended for GUI)

```bash
# Clone Nana library (if not already present)
git clone https://github.com/cnjinhao/nana.git lib/nana

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -G "MinGW Makefiles"    # Windows with MinGW
# OR
cmake .. -G "Visual Studio 17 2022"  # Windows with VS 2022
# OR  
cmake ..                          # Linux/Mac

# Build
cmake --build . --config Release

# Executables created:
#   - TravelBookingConsole.exe
#   - TravelBookingGUI.exe
```

### Option 2: Make Build (Console Only)

```bash
make            # Build console version
make run        # Build and run
make clean      # Remove build files
make debug      # Build with debug symbols
make release    # Build with optimizations
```

### Option 3: Direct Compilation (Console Only)

```bash
g++ -std=c++17 -Wall -Wextra -I include -o TravelBooking src/*.cpp
```

---

## Running the Application

### GUI Version (Recommended)
```bash
# From build directory
./TravelBookingGUI.exe    # Windows
./TravelBookingGUI        # Linux
```

### Console Version
```bash
./TravelBookingConsole.exe    # Windows
./TravelBookingConsole        # Linux
```

---

## 📖 Usage Guide

### Quick Start

1. **Launch** the application (GUI or Console)
2. **Login** with test account: `johndoe` / `john123`
3. **Book a Flight**: Search → Select → Enter details → Confirm
4. **Book a Hotel**: After flight booking, hotels at your destination are pre-filled!
5. **View Bookings**: Check your reservations anytime
6. **Cancel if needed**: Select booking → Cancel

### GUI Navigation

| Section | Description |
|---------|-------------|
| **Flights** | Search by origin/destination, book with passenger details |
| **Hotels** | Search by city/stars, book with check-in/out dates |
| **My Bookings** | View all reservations, cancel if needed |
| **Profile** | View your account information |
| **Login/Logout** | Manage your session |

### Smart Feature: Flight → Hotel Recommendation

When you book a flight:
1. The destination city is saved
2. When you click "Hotels", the destination is auto-filled
3. Search results show hotels in your destination city first
4. One-click booking for complete trip planning!

---

## Test Data

### Test Accounts

| Username | Password | Description |
|----------|----------|-------------|
| `admin` | `admin123` | Administrator account |
| `johndoe` | `john123` | Regular user with sample bookings |
| `janesmith` | `jane456` | Regular user |

### Sample Flights

| Flight | Route | Airline | Price |
|--------|-------|---------|-------|
| AA101 | New York → Los Angeles | American Airlines | $299 |
| UA202 | Chicago → Miami | United Airlines | $249 |
| DL303 | San Francisco → Seattle | Delta Airlines | $179 |

### Sample Hotels

| Hotel | City | Stars | Price/Night |
|-------|------|-------|-------------|
| Grand Hilton | Los Angeles | ⭐⭐⭐⭐⭐ | $350 |
| Marriott Downtown | New York | ⭐⭐⭐⭐ | $275 |
| Holiday Inn Express | Chicago | ⭐⭐⭐ | $120 |

---

## Data File Formats

All data is stored in pipe-delimited text files in the `data/` directory.

### flights.txt
```
# Format: id|name|price|airline|origin|destination|departure|arrival|seats|flightNum
1|Flight to LAX|299.0|American Airlines|New York|Los Angeles|2026-02-15 08:00|2026-02-15 11:30|150|AA101
```

### hotels.txt
```
# Format: id|name|price|location|city|stars|rooms|checkIn|checkOut|amenities
1|Grand Hilton|350.0|Downtown|Los Angeles|5|50|14:00|11:00|WiFi,Pool,Spa,Gym
```

### users.txt
```
# Format: id|username|password|email|fullName|phone
1|johndoe|john123|john@email.com|John Doe|555-1234
```

### bookings.txt
```
# Format: TYPE|bookingId|userId|date|price|...specific fields
FLIGHT|1|2|2026-01-21|299.0|1|2|Economy|John Doe
HOTEL|2|2|2026-01-21|700.0|1|2026-02-15|2026-02-17|1|2|John Doe
```

---

## GUI Styling (Premium Theme)

The GUI uses a custom dark theme with premium styling:

### Color Palette
```cpp
primary:     #2962FF  // Vibrant blue
accent:      #FFAB00  // Gold
background:  #121218  // Deep dark
surface:     #1C1C24  // Card surface
text:        #FFFFFF  // White text
```

### Typography
- **Title**: Segoe UI Semibold, 28pt
- **Heading**: Segoe UI Semibold, 16pt
- **Body**: Segoe UI Regular, 11pt
- **Button**: Segoe UI Semibold, 11pt

---

## Technical Details

### Memory Management
- Uses `std::unique_ptr` for automatic resource cleanup
- RAII pattern for file handles
- No raw `new`/`delete` calls

### Thread Safety
- Single-threaded application
- File I/O is synchronous

### Error Handling
- Input validation for all user input
- Graceful handling of missing data files
- Status messages provide user feedback

---

## Known Limitations

| Limitation | Reason |
|------------|--------|
| Plain text passwords | Demo/educational purposes only |
| Simplified pricing | Hotels charge per booking, not per night |
| No date validation | Accepts any date format |
| Single user session | No session management |

---

## Dependencies

| Dependency | Version | Purpose |
|------------|---------|---------|
| C++ Standard | C++17 | Language standard |
| STL | - | Containers, algorithms, strings |
| Nana | Latest | GUI framework (optional for GUI version) |
| CMake | 3.15+ | Build system (optional) |

