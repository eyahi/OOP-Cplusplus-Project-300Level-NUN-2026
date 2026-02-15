# airline-system-two
# Flight Booking System

A C++/Qt desktop application for managing flight bookings and checking live flight status.  
The system combines a local demo booking engine with a real-time integration to the **AeroDataBox** API (via RapidAPI).

---

## Participants
- Azalea Akpokugbe: 20231719
- Anuri Joy Philemon: 20231749
- Saudah Ahmed Lere: 20232899

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Technology Stack](#technology-stack)
- [Setup and Installation](#setup-and-installation)
- [Configuration (API Key)](#configuration-api-key)
- [How to Use](#how-to-use)
- [Design Decisions and Notes](#design-decisions-and-notes)
- [Possible Improvements](#possible-improvements)

---

## Overview

The Flight Booking System simulates a simple airline booking interface and demonstrates how an existing C++/Qt application can be extended with a third-party REST API.  
Users can browse sample flights, create bookings, cancel flights, and check live flight status by flight number and date using AeroDataBox. 

---

## Features

### Local booking features

- Display a table of **sample flights** with:
  - ID, origin, destination, departure time, and status.
- Search local demo flights by **origin** and **destination** (top search bar). 
- Create a booking for a selected flight (demo dialog).
- Cancel a selected local flight and update its status to **Cancelled** in the table.

### Online flight status (AeroDataBox)

- Enter **flight number** (e.g. `KL641`) and **date**. 
- The app calls the AeroDataBox **Flight by Number + Date** endpoint through RapidAPI:
  - `GET /flights/number/{flightNumber}/{dateLocal}?withAircraftImage=false&withLocation=false&withFlightPlan=false`
- Displays:
  - Status (simplified text)
  - Departure time (local)
  - Arrival time (local)
  - Route (origin → destination) 
  - Note: When the AeroDataBox endpoint returns an array of flight objects (as with `KL641` on `2026-01-21`), the application parses the JSON and maps fields such as origin/destination ICAO codes and scheduled local times into the internal `Flight` model to display in the UI.


---

## Architecture

### Layers and main classes

- **UI Layer (Qt Widgets)**  
  - `MainWindow`
    - Shows the main table of flights and the “Check Flight Status (Online)” panel.
    - Connects user actions (buttons, edits) to the `BookingSystem`.
  - `BookFlightDialog`
    - Simple dialog for confirming a booking for a selected flight. 

- **Domain / Service Layer**
  - `BookingSystem`
    - Central application service that manages:
      - `m_flights`, `m_bookings`, `m_passengers`, `m_seatMaps`.
    - Provides operations:
      - `getFlights()`, `getBookings()`
      - `searchFlights(flightNumber, date)` → delegates to AeroDataBox. 
      - `createBooking`, `cancelFlight`, `assignSeat`, `updateFlightStatus`.
    - Seeds three sample flights and seat maps in `seedSampleData()`.
  - `AeroboxService` / `AeroboxServiceImpl`
    - Interface + implementation for remote calls.
    - `AeroboxServiceImpl` uses `QNetworkAccessManager` to call AeroDataBox.

- **Model Layer**
  - `Flight`: id, origin, destination, departure time, arrival time, status.
  - `Booking`: id, flightId, passengerId, seat, status. 
  - `Passenger`: id, name. 
  - `SeatMap`: simple 2D seat occupancy representation.

---

## Technology Stack

- **Language:** C++17 
- **Framework:** Qt 6 (Core, Widgets, Network) 
- **Build System:** CMake (minimum 3.16) 
- **HTTP / JSON:** `QNetworkAccessManager`, `QNetworkRequest`, `QNetworkReply`, `QJsonDocument`, `QJsonObject`, `QJsonArray`. 
- **External API:** AeroDataBox via RapidAPI (`aerodatabox.p.rapidapi.com`). 

---

## Setup and Installation

### Prerequisites

- C++17‑compatible compiler (e.g. MSVC, Clang, or GCC). 
- Qt 6 with **Core**, **Widgets**, and **Network** modules installed. 
- CMake ≥ 3.16.
- A RapidAPI account with access to **AeroDataBox** (for `x-rapidapi-key`).

### Building the project

From the project root:

```bash
mkdir build
cd build
cmake ..
cmake --build .
