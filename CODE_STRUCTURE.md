# TravelBook Application - Code Structure & OOP Analysis

This document provides a comprehensive breakdown of the TravelBook application's code structure, explaining the class hierarchy, OOP principles used, and the flow of execution when actions are performed.

---

## 🚀 Application Startup Flow

The application has **two entry points**:

### 1. Console Mode (`src/main.cpp`)
```
main() → BookingSystem (created) → Menu (created) → menu.run()
```

### 2. GUI Mode (`src/gui/gui_main.cpp`)
```
WinMain() → BookingSystem (created) → MainWindow (created) → mainWindow.run()
```

When you run `TravelBooking.exe`, this is what happens:

| Step | What Happens | Class Called | OOP Characteristics |
|------|-------------|--------------|---------------------|
| 1 | App starts | `WinMain()` | - |
| 2 | Create core system | `BookingSystem` | **Encapsulation**, **Composition** |
| 3 | Load data from files | `loadAllData()` → creates `Flight`, `Hotel`, `User`, `Booking` objects | **Polymorphism** (Bookings stored as base pointers) |
| 4 | Create window | `MainWindow` | **Inheritance** (extends `nana::form`) |
| 5 | Show GUI | `mainWindow.run()` | - |

---

## 📊 Class Hierarchy & OOP Characteristics

### **Hierarchy Diagram**

```
                        ┌─────────────────┐
                        │    Bookable     │  ◀── ABSTRACT BASE CLASS
                        │  (pure virtual) │
                        └────────┬────────┘
                                 │
              ┌──────────────────┴──────────────────┐
              │                                      │
       ┌──────▼──────┐                       ┌──────▼──────┐
       │   Flight    │                       │    Hotel    │
       │  (derived)  │                       │  (derived)  │
       └─────────────┘                       └─────────────┘


                        ┌─────────────────┐
                        │    Booking      │  ◀── ABSTRACT BASE CLASS
                        │  (pure virtual) │
                        └────────┬────────┘
                                 │
              ┌──────────────────┴──────────────────┐
              │                                      │
       ┌──────▼──────┐                       ┌──────▼──────┐
       │FlightBooking│                       │HotelBooking │
       │  (derived)  │                       │  (derived)  │
       └─────────────┘                       └─────────────┘


       ┌─────────────────┐
       │   nana::form    │  ◀── External library base
       └────────┬────────┘
                │
       ┌────────▼────────┐
       │   MainWindow    │
       │   (derived)     │
       └─────────────────┘
```

---

## 🔍 Detailed OOP Characteristics Per Class

### 1. **`Bookable`** (Abstract Base Class)

| OOP Concept | Implementation |
|-------------|----------------|
| **Abstraction** | Pure virtual functions: `displayInfo()`, `getType()`, `isAvailable()` |
| **Encapsulation** | `protected` members: `id`, `name`, `price`, `location` |
| **Virtual Destructor** | `virtual ~Bookable() = default;` for proper polymorphic cleanup |

```cpp
// Pure virtual functions (= 0 means MUST be overridden)
virtual void displayInfo() const = 0;
virtual std::string getType() const = 0;
virtual bool isAvailable() const = 0;
```

---

### 2. **`Flight`** (Derived from `Bookable`)

| OOP Concept | Implementation |
|-------------|----------------|
| **Inheritance** | `class Flight : public Bookable` |
| **Polymorphism** | Overrides `displayInfo()`, `getType()`, `isAvailable()` |
| **Encapsulation** | Private members: `airline`, `origin`, `destination`, `availableSeats`, etc. |

```cpp
// Polymorphic overrides
void displayInfo() const override;   // Shows flight-specific info
std::string getType() const override; // Returns "Flight"
bool isAvailable() const override;   // Checks availableSeats > 0
```

---

### 3. **`Hotel`** (Derived from `Bookable`)

| OOP Concept | Implementation |
|-------------|----------------|
| **Inheritance** | `class Hotel : public Bookable` |
| **Polymorphism** | Overrides `displayInfo()`, `getType()`, `isAvailable()` |
| **Encapsulation** | Private members: `starRating`, `availableRooms`, `amenities`, etc. |

```cpp
// Polymorphic overrides
void displayInfo() const override;   // Shows hotel-specific info
std::string getType() const override; // Returns "Hotel"
bool isAvailable() const override;   // Checks availableRooms > 0
```

---

### 4. **`Booking`** (Abstract Base Class)

| OOP Concept | Implementation |
|-------------|----------------|
| **Abstraction** | Pure virtual functions: `getDetails()`, `getBookingType()`, `displayBooking()`, `getBookableId()` |
| **Encapsulation** | `protected` members: `bookingId`, `userId`, `totalPrice`, `status` |
| **Enumeration** | `BookingStatus` enum for type-safe status management |

---

### 5. **`FlightBooking`** (Derived from `Booking`)

| OOP Concept | Implementation |
|-------------|----------------|
| **Inheritance** | `class FlightBooking : public Booking` |
| **Polymorphism** | Overrides all pure virtual functions from `Booking` |
| **Encapsulation** | Private: `flightId`, `numPassengers`, `seatClass`, `passengerName` |

---

### 6. **`HotelBooking`** (Derived from `Booking`)

| OOP Concept | Implementation |
|-------------|----------------|
| **Inheritance** | `class HotelBooking : public Booking` |
| **Polymorphism** | Overrides all pure virtual functions from `Booking` |
| **Encapsulation** | Private: `hotelId`, `checkInDate`, `checkOutDate`, `numRooms` |

---

### 7. **`BookingSystem`** (Controller/Manager Class)

| OOP Concept | Implementation |
|-------------|----------------|
| **Composition** | Contains vectors of `Flight*`, `Hotel*`, `User`, `Booking*` |
| **Encapsulation** | All data private, accessed via public methods |
| **Polymorphism** | Uses `unique_ptr<Booking>` to store both `FlightBooking` and `HotelBooking` |
| **Smart Pointers** | `std::unique_ptr` for automatic memory management |

```cpp
// Polymorphic storage - can hold FlightBooking or HotelBooking
std::vector<std::unique_ptr<Booking>> bookings;
```

---

### 8. **`MainWindow`** (GUI Controller)

| OOP Concept | Implementation |
|-------------|----------------|
| **Inheritance** | `class MainWindow : public nana::form` |
| **Encapsulation** | Private widget members, public interface |
| **Composition** | Contains reference to `BookingSystem` |

---

### 9. **`User`** (Entity Class)

| OOP Concept | Implementation |
|-------------|----------------|
| **Encapsulation** | All data private with getter/setter methods |
| No inheritance | Standalone class |

---

## 🎯 Action Flow Examples

### **Action: User clicks "Search Flights"**

```
MainWindow::searchFlights()
    │
    ├─► BookingSystem::searchFlights(origin, destination)
    │       │
    │       └─► Iterates through vector<unique_ptr<Flight>>
    │           Returns: vector<Flight*> (polymorphic access)
    │
    └─► Display results in listbox (using Flight::getters)
```

**OOP in action:**
- **Encapsulation**: `BookingSystem` hides the internal storage
- **Polymorphism**: Flights stored as `unique_ptr<Flight>` but could be accessed via `Bookable*`

---

### **Action: User books a flight**

```
MainWindow::bookSelectedFlight()
    │
    ├─► BookingSystem::bookFlight(flightId, passengers, class, name)
    │       │
    │       ├─► Find Flight by ID
    │       ├─► Flight::bookSeats(numPassengers)  ◀── Method on derived class
    │       │
    │       ├─► Create new FlightBooking object
    │       │       └─► FlightBooking : public Booking  ◀── INHERITANCE
    │       │
    │       └─► Add to vector<unique_ptr<Booking>>  ◀── POLYMORPHISM
    │
    └─► Update UI, show confirmation
```

**OOP in action:**
- **Inheritance**: `FlightBooking` inherits from `Booking`
- **Polymorphism**: Stored as `unique_ptr<Booking>` despite being `FlightBooking`
- **Encapsulation**: Booking details hidden, accessed via virtual methods

---

### **Action: View My Bookings**

```
MainWindow::showBookingsView()
    │
    ├─► BookingSystem::getUserBookings()
    │       │
    │       └─► Returns vector<Booking*>
    │           (Could be FlightBooking* or HotelBooking*)
    │
    └─► For each booking:
            booking->getBookingType()    ◀── POLYMORPHISM
            booking->getDetails()        ◀── Calls correct derived class method
```

**Polymorphism in action:**
- Same method call (`getDetails()`) produces different results depending on whether it's a `FlightBooking` or `HotelBooking`

---

## 📌 Summary Table

| Class | Base Class | Key OOP Concepts |
|-------|------------|-----------------|
| `Bookable` | - | **Abstract base**, Encapsulation, Virtual destructor |
| `Flight` | `Bookable` | **Inheritance**, **Polymorphism** (override), Encapsulation |
| `Hotel` | `Bookable` | **Inheritance**, **Polymorphism** (override), Encapsulation |
| `Booking` | - | **Abstract base**, Encapsulation |
| `FlightBooking` | `Booking` | **Inheritance**, **Polymorphism** (override) |
| `HotelBooking` | `Booking` | **Inheritance**, **Polymorphism** (override) |
| `BookingSystem` | - | **Composition**, Encapsulation, Uses polymorphism |
| `MainWindow` | `nana::form` | **Inheritance** from GUI library |
| `User` | - | **Encapsulation** |
| `Menu` | - | **Encapsulation**, **Composition** |

---

## 📁 File Structure

```
OOP_CPP_Project/
├── include/                    # Header files
│   ├── Bookable.h             # Abstract base class
│   ├── Booking.h              # Abstract base class
│   ├── BookingSystem.h        # Main controller
│   ├── Flight.h               # Derived from Bookable
│   ├── FlightBooking.h        # Derived from Booking
│   ├── Hotel.h                # Derived from Bookable
│   ├── HotelBooking.h         # Derived from Booking
│   ├── Menu.h                 # Console UI
│   ├── User.h                 # User entity
│   └── gui/
│       ├── MainWindow.h       # GUI main window
│       └── Styles.h           # GUI styling
├── src/                       # Implementation files
│   ├── main.cpp               # Console entry point
│   ├── Bookable.cpp
│   ├── Booking.cpp
│   ├── BookingSystem.cpp
│   ├── Flight.cpp
│   ├── FlightBooking.cpp
│   ├── Hotel.cpp
│   ├── HotelBooking.cpp
│   ├── Menu.cpp
│   ├── User.cpp
│   └── gui/
│       ├── gui_main.cpp       # GUI entry point
│       └── MainWindow.cpp
├── data/                      # Data files
│   ├── flights.txt
│   ├── hotels.txt
│   ├── users.txt
│   └── bookings.txt
└── lib/                       # External libraries
    └── nana/                  # Nana GUI library
```

---

*Document generated: 2026-01-23*
