/**
 * @file MainWindow.cpp
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Full implementation of the main application window with all features
 */

#include "gui/MainWindow.h"
#include "gui/Styles.h"
#include <nana/gui/msgbox.hpp>

MainWindow::MainWindow(BookingSystem& system)
    : nana::form(nana::API::make_center(1200, 800)), bookingSystem(system) {
    
    caption("TravelBook - Premium Travel Booking");
    bgcolor(styles::colors::background);
    
    applyPremiumStyling();
    setupLayout();
    setupEventHandlers();
    
    updateUserDisplay();
}

void MainWindow::applyPremiumStyling() {
    // Title with premium typography
    titleLabel.bgcolor(styles::colors::background);
    titleLabel.fgcolor(styles::colors::primaryLight);
    titleLabel.typeface(styles::fonts::title());
    titleLabel.text_align(nana::align::left, nana::align_v::center);
    
    // Subtitle - lighter weight
    subtitleLabel.bgcolor(styles::colors::background);
    subtitleLabel.fgcolor(styles::colors::textMuted);
    subtitleLabel.typeface(styles::fonts::subtitle());
    
    // Navigation buttons with consistent styling
    auto styleNavButton = [](nana::button& btn, bool isActive = false) {
        btn.bgcolor(isActive ? styles::colors::primary : styles::colors::surfaceLight);
        btn.fgcolor(styles::colors::textPrimary);
        btn.typeface(styles::fonts::button());
    };
    
    styleNavButton(btnFlights, true);
    styleNavButton(btnHotels);
    styleNavButton(btnMyBookings);
    styleNavButton(btnProfile);
    
    // Accent login button
    btnLogin.bgcolor(styles::colors::accent);
    btnLogin.fgcolor(styles::colors::background);
    btnLogin.typeface(styles::fonts::button());
    
    // Content panel
    contentPanel.bgcolor(styles::colors::surface);
    
    // Status bar
    statusLabel.bgcolor(styles::colors::surfaceLight);
    statusLabel.fgcolor(styles::colors::textSecondary);
    statusLabel.typeface(styles::fonts::body());
    statusLabel.text_align(nana::align::left, nana::align_v::center);
    
    // User label
    userLabel.bgcolor(styles::colors::background);
    userLabel.fgcolor(styles::colors::textMuted);
    userLabel.typeface(styles::fonts::body());
    userLabel.text_align(nana::align::right, nana::align_v::center);
}

void MainWindow::setupLayout() {
    layout.div(R"(
        <vertical
            <weight=80 header
                <weight=20>
                <vertical
                    <weight=45 <title><weight=200 user>>
                    <subtitle>
                >
                <weight=20>
            >
            <weight=50 nav
                <weight=20>
                <weight=120 btnFlights>
                <weight=10>
                <weight=120 btnHotels>
                <weight=10>
                <weight=120 btnMyBookings>
                <weight=10>
                <weight=120 btnProfile>
                <>
                <weight=100 btnLogin>
                <weight=20>
            >
            <weight=10>
            <content>
            <weight=30 status>
        >
    )");
    
    layout["title"] << titleLabel;
    layout["user"] << userLabel;
    layout["subtitle"] << subtitleLabel;
    layout["btnFlights"] << btnFlights;
    layout["btnHotels"] << btnHotels;
    layout["btnMyBookings"] << btnMyBookings;
    layout["btnProfile"] << btnProfile;
    layout["btnLogin"] << btnLogin;
    layout["content"] << contentPanel;
    layout["status"] << statusLabel;
    
    layout.collocate();
}

void MainWindow::setupEventHandlers() {
    btnFlights.events().click([this] { showFlightsView(); });
    btnHotels.events().click([this] { showHotelsView(); });
    
    btnMyBookings.events().click([this] {
        if (!bookingSystem.isLoggedIn()) {
            showMessage("Login Required", "Please login to view your bookings.");
            return;
        }
        showBookingsView();
    });
    
    btnProfile.events().click([this] {
        if (!bookingSystem.isLoggedIn()) {
            showMessage("Login Required", "Please login to view your profile.");
            return;
        }
        showProfileView();
    });
    
    btnLogin.events().click([this] {
        if (bookingSystem.isLoggedIn()) {
            bookingSystem.logout();
            btnLogin.caption("Login");
            updateUserDisplay();
            statusLabel.caption("Logged out successfully");
        } else {
            showLoginDialog();
        }
    });
    
    showFlightsView();
}

void MainWindow::updateNavButtons(int active) {
    btnFlights.bgcolor(active == 0 ? styles::colors::primary : styles::colors::surface);
    btnHotels.bgcolor(active == 1 ? styles::colors::primary : styles::colors::surface);
    btnMyBookings.bgcolor(active == 2 ? styles::colors::primary : styles::colors::surface);
    btnProfile.bgcolor(active == 3 ? styles::colors::primary : styles::colors::surface);
}

void MainWindow::clearViewWidgets() {
    // Flight widgets
    flightLblFrom.reset();
    flightTxtFrom.reset();
    flightLblTo.reset();
    flightTxtTo.reset();
    flightBtnSearch.reset();
    flightResultsLabel.reset();
    flightList.reset();
    flightLblPassengers.reset();
    flightSpnPassengers.reset();
    flightLblClass.reset();
    flightCmbClass.reset();
    flightLblName.reset();
    flightTxtName.reset();
    flightBtnBook.reset();
    
    // Hotel widgets
    hotelLblSuggestion.reset();
    hotelLblCity.reset();
    hotelTxtCity.reset();
    hotelLblStars.reset();
    hotelCmbStars.reset();
    hotelBtnSearch.reset();
    hotelResultsLabel.reset();
    hotelList.reset();
    hotelLblCheckIn.reset();
    hotelTxtCheckIn.reset();
    hotelLblCheckOut.reset();
    hotelTxtCheckOut.reset();
    hotelLblRooms.reset();
    hotelSpnRooms.reset();
    hotelLblGuests.reset();
    hotelSpnGuests.reset();
    hotelLblGuestName.reset();
    hotelTxtGuestName.reset();
    hotelBtnBook.reset();
    
    // Bookings widgets
    bookingsTitle.reset();
    bookingsList.reset();
    bookingsBtnCancel.reset();
    
    // Profile widgets
    profileTitle.reset();
    profileLbl1.reset(); profileVal1.reset();
    profileLbl2.reset(); profileVal2.reset();
    profileLbl3.reset(); profileVal3.reset();
    profileLbl4.reset(); profileVal4.reset();
    profileLbl5.reset(); profileVal5.reset();
}

void MainWindow::showFlightsView() {
    updateNavButtons(0);
    clearViewWidgets();
    
    // Improved layout with better spacing and centered book button
    contentLayout.div(R"(
        <vertical margin=20
            <weight=40 searchRow
                <weight=70 lblFrom><weight=8><weight=200 txtFrom>
                <weight=25>
                <weight=70 lblTo><weight=8><weight=200 txtTo>
                <weight=25>
                <weight=120 btnSearch>
                <>
            >
            <weight=20>
            <weight=28 resultsLabel>
            <weight=8>
            <flightList>
            <weight=20>
            <weight=45 bookingRow
                <weight=90 lblPass><weight=80 spnPass>
                <weight=25>
                <weight=65 lblClass><weight=120 cmbClass>
                <weight=25>
                <weight=110 lblName><weight=200 txtName>
                <>
            >
            <weight=15>
            <weight=45 bookBtnRow
                <>
                <weight=200 btnBook>
                <>
            >
        >
    )");
    
    // Create widgets
    flightLblFrom = std::make_unique<nana::label>(contentPanel, "From:");
    flightTxtFrom = std::make_unique<nana::textbox>(contentPanel);
    flightLblTo = std::make_unique<nana::label>(contentPanel, "To:");
    flightTxtTo = std::make_unique<nana::textbox>(contentPanel);
    flightBtnSearch = std::make_unique<nana::button>(contentPanel, "Search");
    flightResultsLabel = std::make_unique<nana::label>(contentPanel, "Available Flights:");
    flightList = std::make_unique<nana::listbox>(contentPanel);
    flightLblPassengers = std::make_unique<nana::label>(contentPanel, "Passengers:");
    flightSpnPassengers = std::make_unique<nana::spinbox>(contentPanel);
    flightLblClass = std::make_unique<nana::label>(contentPanel, "Class:");
    flightCmbClass = std::make_unique<nana::combox>(contentPanel);
    flightLblName = std::make_unique<nana::label>(contentPanel, "Name:");
    flightTxtName = std::make_unique<nana::textbox>(contentPanel);
    flightBtnBook = std::make_unique<nana::button>(contentPanel, "Book Flight");
    
    // Style labels with premium typography
    styles::applyBodyLabel(*flightLblFrom);
    styles::applyBodyLabel(*flightLblTo);
    styles::applyBodyLabel(*flightLblPassengers);
    styles::applyBodyLabel(*flightLblClass);
    styles::applyBodyLabel(*flightLblName);
    
    // Results heading
    styles::applyHeadingLabel(*flightResultsLabel);
    
    // Buttons
    styles::applyPrimaryButton(*flightBtnSearch);
    styles::applyAccentButton(*flightBtnBook);
    
    // Listbox with alternating row colors
    styles::applyListboxStyle(*flightList);
    flightList->append_header("ID", 50);
    flightList->append_header("Flight #", 95);
    flightList->append_header("Airline", 140);
    flightList->append_header("From", 110);
    flightList->append_header("To", 110);
    flightList->append_header("Departure", 145);
    flightList->append_header("Price", 85);
    flightList->append_header("Seats", 65);
    
    // Configure inputs
    flightSpnPassengers->range(1, 10, 1);
    flightSpnPassengers->value("1");
    
    flightCmbClass->push_back("Economy");
    flightCmbClass->push_back("Business");
    flightCmbClass->push_back("First");
    flightCmbClass->option(0);
    
    // Layout
    contentLayout["lblFrom"] << *flightLblFrom;
    contentLayout["txtFrom"] << *flightTxtFrom;
    contentLayout["lblTo"] << *flightLblTo;
    contentLayout["txtTo"] << *flightTxtTo;
    contentLayout["btnSearch"] << *flightBtnSearch;
    contentLayout["resultsLabel"] << *flightResultsLabel;
    contentLayout["flightList"] << *flightList;
    contentLayout["lblPass"] << *flightLblPassengers;
    contentLayout["spnPass"] << *flightSpnPassengers;
    contentLayout["lblClass"] << *flightLblClass;
    contentLayout["cmbClass"] << *flightCmbClass;
    contentLayout["lblName"] << *flightLblName;
    contentLayout["txtName"] << *flightTxtName;
    contentLayout["btnBook"] << *flightBtnBook;
    contentLayout.collocate();
    
    // Events
    flightBtnSearch->events().click([this] { searchFlights(); });
    flightBtnBook->events().click([this] { bookSelectedFlight(); });
    
    // Update passenger limit when flight is selected
    flightList->events().selected([this](const nana::arg_listbox& arg) {
        if (arg.item.selected() && flightSpnPassengers) {
            auto item = flightList->at(0).at(arg.item.pos().item);
            int seats = std::stoi(item.text(7)); // Seats column
            if (seats > 0) {
                flightSpnPassengers->range(1, seats, 1);
                flightSpnPassengers->value("1");
            }
        }
    });
    
    // Initial search
    searchFlights();
}

void MainWindow::searchFlights() {
    if (!flightList) return;
    
    flightList->clear();
    std::string from = flightTxtFrom ? flightTxtFrom->caption() : "";
    std::string to = flightTxtTo ? flightTxtTo->caption() : "";
    
    auto flights = bookingSystem.searchFlights(from, to);
    
    if (flights.empty()) {
        statusLabel.caption("No flights found.");
        return;
    }
    
    auto cat = flightList->at(0);
    for (auto* flight : flights) {
        cat.append({
            std::to_string(flight->getId()),
            flight->getFlightNumber(),
            flight->getAirline(),
            flight->getOrigin(),
            flight->getDestination(),
            flight->getDepartureTime(),
            "$" + std::to_string(static_cast<int>(flight->getPrice())),
            std::to_string(flight->getAvailableSeats())
        });
    }
    
    statusLabel.caption("Found " + std::to_string(flights.size()) + " flights");
}

void MainWindow::bookSelectedFlight() {
    if (!bookingSystem.isLoggedIn()) {
        showMessage("Login Required", "Please login to book a flight.");
        return;
    }
    
    if (!flightList) return;
    
    auto sel = flightList->selected();
    if (sel.empty()) {
        showMessage("Select Flight", "Please select a flight to book.");
        return;
    }
    
    auto item = flightList->at(0).at(sel[0].item);
    int flightId = std::stoi(item.text(0));
    int passengers = flightSpnPassengers ? std::stoi(flightSpnPassengers->value()) : 1;
    std::string seatClass = flightCmbClass ? flightCmbClass->caption() : "Economy";
    std::string name = flightTxtName ? flightTxtName->caption() : "";
    
    if (name.empty()) {
        showMessage("Name Required", "Please enter the passenger name.");
        return;
    }
    
    if (bookingSystem.bookFlight(flightId, passengers, seatClass, name)) {
        std::string destination = bookingSystem.getLastBookedDestination();
        nana::msgbox mb(*this, "Success!");
        mb << "Flight booked successfully!\n\nClick OK to browse hotels in " << destination << ".";
        mb.show();
        
        // Auto-navigate to hotels view with destination pre-filled
        showHotelsView();
    } else {
        showMessage("Error", "Failed to book flight. Not enough seats available.");
    }
}

void MainWindow::showHotelsView() {
    updateNavButtons(1);
    clearViewWidgets();
    
    // Improved layout with better spacing and centered book button
    contentLayout.div(R"(
        <vertical margin=20
            <weight=28 suggestionRow>
            <weight=12>
            <weight=40 searchRow
                <weight=60 lblCity><weight=8><weight=200 txtCity>
                <weight=25>
                <weight=90 lblStars><weight=8><weight=100 cmbStars>
                <weight=25>
                <weight=120 btnSearch>
                <>
            >
            <weight=20>
            <weight=28 resultsLabel>
            <weight=8>
            <hotelList>
            <weight=20>
            <weight=42 bookingRow1
                <weight=80 lblCheckIn><weight=130 txtCheckIn>
                <weight=25>
                <weight=90 lblCheckOut><weight=130 txtCheckOut>
                <weight=25>
                <weight=65 lblRooms><weight=70 spnRooms>
                <weight=25>
                <weight=65 lblGuests><weight=70 spnGuests>
                <>
            >
            <weight=15>
            <weight=42 bookingRow2
                <weight=100 lblGuestName><weight=200 txtGuestName>
                <>
            >
            <weight=15>
            <weight=45 bookBtnRow
                <>
                <weight=200 btnBook>
                <>
            >
        >
    )");
    
    // Create widgets
    hotelLblSuggestion = std::make_unique<nana::label>(contentPanel);
    hotelLblCity = std::make_unique<nana::label>(contentPanel, "City:");
    hotelTxtCity = std::make_unique<nana::textbox>(contentPanel);
    hotelLblStars = std::make_unique<nana::label>(contentPanel, "Min Stars:");
    hotelCmbStars = std::make_unique<nana::combox>(contentPanel);
    hotelBtnSearch = std::make_unique<nana::button>(contentPanel, "Search");
    hotelResultsLabel = std::make_unique<nana::label>(contentPanel, "Available Hotels:");
    hotelList = std::make_unique<nana::listbox>(contentPanel);
    hotelLblCheckIn = std::make_unique<nana::label>(contentPanel, "Check-in:");
    hotelTxtCheckIn = std::make_unique<nana::textbox>(contentPanel);
    hotelLblCheckOut = std::make_unique<nana::label>(contentPanel, "Check-out:");
    hotelTxtCheckOut = std::make_unique<nana::textbox>(contentPanel);
    hotelLblRooms = std::make_unique<nana::label>(contentPanel, "Rooms:");
    hotelSpnRooms = std::make_unique<nana::spinbox>(contentPanel);
    hotelLblGuests = std::make_unique<nana::label>(contentPanel, "Guests:");
    hotelSpnGuests = std::make_unique<nana::spinbox>(contentPanel);
    hotelLblGuestName = std::make_unique<nana::label>(contentPanel, "Guest Name:");
    hotelTxtGuestName = std::make_unique<nana::textbox>(contentPanel);
    hotelBtnBook = std::make_unique<nana::button>(contentPanel, "Book Hotel");
    
    // Style labels with premium typography
    styles::applyBodyLabel(*hotelLblCity);
    styles::applyBodyLabel(*hotelLblStars);
    styles::applyBodyLabel(*hotelLblCheckIn);
    styles::applyBodyLabel(*hotelLblCheckOut);
    styles::applyBodyLabel(*hotelLblRooms);
    styles::applyBodyLabel(*hotelLblGuests);
    styles::applyBodyLabel(*hotelLblGuestName);
    
    // Suggestion label - accent color
    hotelLblSuggestion->bgcolor(styles::colors::surface);
    hotelLblSuggestion->fgcolor(styles::colors::accentLight);
    hotelLblSuggestion->typeface(styles::fonts::bodyBold());
    
    // Results heading
    styles::applyHeadingLabel(*hotelResultsLabel);
    
    // Buttons
    styles::applyPrimaryButton(*hotelBtnSearch);
    styles::applyAccentButton(*hotelBtnBook);
    
    // Show destination suggestion if available
    if (!bookingSystem.getLastBookedDestination().empty()) {
        hotelLblSuggestion->caption("Recommended for you: Hotels in " + bookingSystem.getLastBookedDestination());
        hotelTxtCity->caption(bookingSystem.getLastBookedDestination());
    }
    
    // Listbox with alternating row colors
    styles::applyListboxStyle(*hotelList);
    hotelList->append_header("ID", 50);
    hotelList->append_header("Hotel Name", 190);
    hotelList->append_header("City", 110);
    hotelList->append_header("Stars", 65);
    hotelList->append_header("$/Night", 80);
    hotelList->append_header("Rooms", 65);
    hotelList->append_header("Amenities", 230);
    
    // Configure inputs
    hotelCmbStars->push_back("Any");
    hotelCmbStars->push_back("3+");
    hotelCmbStars->push_back("4+");
    hotelCmbStars->push_back("5");
    hotelCmbStars->option(0);
    
    hotelSpnRooms->range(1, 10, 1);
    hotelSpnRooms->value("1");
    hotelSpnGuests->range(1, 20, 1);
    hotelSpnGuests->value("2");
    
    hotelTxtCheckIn->caption("2026-02-15");
    hotelTxtCheckOut->caption("2026-02-18");
    
    // Layout
    contentLayout["suggestionRow"] << *hotelLblSuggestion;
    contentLayout["lblCity"] << *hotelLblCity;
    contentLayout["txtCity"] << *hotelTxtCity;
    contentLayout["lblStars"] << *hotelLblStars;
    contentLayout["cmbStars"] << *hotelCmbStars;
    contentLayout["btnSearch"] << *hotelBtnSearch;
    contentLayout["resultsLabel"] << *hotelResultsLabel;
    contentLayout["hotelList"] << *hotelList;
    contentLayout["lblCheckIn"] << *hotelLblCheckIn;
    contentLayout["txtCheckIn"] << *hotelTxtCheckIn;
    contentLayout["lblCheckOut"] << *hotelLblCheckOut;
    contentLayout["txtCheckOut"] << *hotelTxtCheckOut;
    contentLayout["lblRooms"] << *hotelLblRooms;
    contentLayout["spnRooms"] << *hotelSpnRooms;
    contentLayout["lblGuests"] << *hotelLblGuests;
    contentLayout["spnGuests"] << *hotelSpnGuests;
    contentLayout["lblGuestName"] << *hotelLblGuestName;
    contentLayout["txtGuestName"] << *hotelTxtGuestName;
    contentLayout["btnBook"] << *hotelBtnBook;
    contentLayout.collocate();
    
    // Events
    hotelBtnSearch->events().click([this] { searchHotels(); });
    hotelBtnBook->events().click([this] { bookSelectedHotel(); });
    
    // Update room/guest limits when hotel is selected
    hotelList->events().selected([this](const nana::arg_listbox& arg) {
        if (arg.item.selected() && hotelSpnRooms && hotelSpnGuests) {
            auto item = hotelList->at(0).at(arg.item.pos().item);
            int rooms = std::stoi(item.text(5)); // Rooms column
            if (rooms > 0) {
                hotelSpnRooms->range(1, rooms, 1);
                hotelSpnRooms->value("1");
                // Max guests = rooms * 4 (reasonable estimate)
                hotelSpnGuests->range(1, rooms * 4, 1);
                hotelSpnGuests->value("2");
            }
        }
    });
    
    searchHotels();
}

void MainWindow::searchHotels() {
    if (!hotelList) return;
    
    hotelList->clear();
    std::string city = hotelTxtCity ? hotelTxtCity->caption() : "";
    
    int minStars = 0;
    if (hotelCmbStars) {
        std::string opt = hotelCmbStars->caption();
        if (opt == "3+") minStars = 3;
        else if (opt == "4+") minStars = 4;
        else if (opt == "5") minStars = 5;
    }
    
    auto hotels = (minStars > 0) ? 
        bookingSystem.searchHotelsByRating(city, minStars) :
        bookingSystem.searchHotels(city);
    
    if (hotels.empty()) {
        statusLabel.caption("No hotels found.");
        return;
    }
    
    auto cat = hotelList->at(0);
    for (auto* hotel : hotels) {
        std::string stars(hotel->getStarRating(), '*');
        std::string amenities;
        for (const auto& a : hotel->getAmenities()) {
            if (!amenities.empty()) amenities += ", ";
            amenities += a;
        }
        
        cat.append({
            std::to_string(hotel->getId()),
            hotel->getName(),
            hotel->getCity(),
            stars,
            "$" + std::to_string(static_cast<int>(hotel->getPrice())),
            std::to_string(hotel->getAvailableRooms()),
            amenities
        });
    }
    
    statusLabel.caption("Found " + std::to_string(hotels.size()) + " hotels");
}

void MainWindow::bookSelectedHotel() {
    if (!bookingSystem.isLoggedIn()) {
        showMessage("Login Required", "Please login to book a hotel.");
        return;
    }
    
    if (!hotelList) return;
    
    auto sel = hotelList->selected();
    if (sel.empty()) {
        showMessage("Select Hotel", "Please select a hotel to book.");
        return;
    }
    
    auto item = hotelList->at(0).at(sel[0].item);
    int hotelId = std::stoi(item.text(0));
    std::string checkIn = hotelTxtCheckIn ? hotelTxtCheckIn->caption() : "";
    std::string checkOut = hotelTxtCheckOut ? hotelTxtCheckOut->caption() : "";
    int rooms = hotelSpnRooms ? std::stoi(hotelSpnRooms->value()) : 1;
    int guests = hotelSpnGuests ? std::stoi(hotelSpnGuests->value()) : 2;
    std::string name = hotelTxtGuestName ? hotelTxtGuestName->caption() : "";
    
    if (name.empty()) {
        showMessage("Name Required", "Please enter the guest name.");
        return;
    }
    
    if (bookingSystem.bookHotel(hotelId, checkIn, checkOut, rooms, guests, name)) {
        showMessage("Success!", "Hotel booked successfully!");
        statusLabel.caption("Hotel booked successfully!");
        searchHotels();
    } else {
        showMessage("Error", "Failed to book hotel.");
    }
}

void MainWindow::showBookingsView() {
    updateNavButtons(2);
    clearViewWidgets();
    
    contentLayout.div(R"(
        <vertical margin=20
            <weight=35 titleLabel>
            <weight=20>
            <bookingsList>
            <weight=20>
            <weight=45 buttonRow
                <>
                <weight=180 btnCancel>
                <>
            >
        >
    )");
    
    bookingsTitle = std::make_unique<nana::label>(contentPanel, "My Bookings");
    bookingsList = std::make_unique<nana::listbox>(contentPanel);
    bookingsBtnCancel = std::make_unique<nana::button>(contentPanel, "Cancel Booking");
    
    // Premium styling
    styles::applyHeadingLabel(*bookingsTitle);
    bookingsTitle->typeface(nana::paint::font("Segoe UI", 18, {600}));
    
    // Listbox with alternating rows
    styles::applyListboxStyle(*bookingsList);
    bookingsList->append_header("ID", 65);
    bookingsList->append_header("Type", 85);
    bookingsList->append_header("Details", 380);
    bookingsList->append_header("Date", 110);
    bookingsList->append_header("Price", 90);
    bookingsList->append_header("Status", 100);
    
    // Danger button for cancel
    styles::applyDangerButton(*bookingsBtnCancel);
    
    contentLayout["titleLabel"] << *bookingsTitle;
    contentLayout["bookingsList"] << *bookingsList;
    contentLayout["btnCancel"] << *bookingsBtnCancel;
    contentLayout.collocate();
    
    // Load bookings
    auto bookings = bookingSystem.getUserBookings();
    auto cat = bookingsList->at(0);
    
    for (auto* booking : bookings) {
        std::string details;
        if (booking->getBookingType() == "Flight") {
            auto* fb = dynamic_cast<FlightBooking*>(booking);
            if (fb) {
                auto* flight = bookingSystem.findFlightById(fb->getFlightId());
                if (flight) {
                    details = flight->getOrigin() + " -> " + flight->getDestination() + 
                             " (" + fb->getSeatClass() + ", " + 
                             std::to_string(fb->getNumPassengers()) + " pax)";
                }
            }
        } else {
            auto* hb = dynamic_cast<HotelBooking*>(booking);
            if (hb) {
                auto* hotel = bookingSystem.findHotelById(hb->getHotelId());
                if (hotel) {
                    details = hotel->getName() + " (" + hb->getCheckInDate() + 
                             " to " + hb->getCheckOutDate() + ")";
                }
            }
        }
        
        cat.append({
            std::to_string(booking->getBookingId()),
            booking->getBookingType(),
            details,
            booking->getBookingDate(),
            "$" + std::to_string(static_cast<int>(booking->getTotalPrice())),
            booking->getStatusString()
        });
    }
    
    statusLabel.caption("You have " + std::to_string(bookings.size()) + " booking(s)");
    
    bookingsBtnCancel->events().click([this] { cancelSelectedBooking(); });
}

void MainWindow::cancelSelectedBooking() {
    if (!bookingsList) return;
    
    auto sel = bookingsList->selected();
    if (sel.empty()) {
        showMessage("Select Booking", "Please select a booking to cancel.");
        return;
    }
    
    auto item = bookingsList->at(0).at(sel[0].item);
    int bookingId = std::stoi(item.text(0));
    
    if (bookingSystem.cancelBooking(bookingId)) {
        showMessage("Success", "Booking cancelled!");
        showBookingsView();
    } else {
        showMessage("Error", "Failed to cancel booking.");
    }
}

void MainWindow::showProfileView() {
    updateNavButtons(3);
    clearViewWidgets();
    
    if (!bookingSystem.isLoggedIn()) return;
    User* user = bookingSystem.getCurrentUser();
    
    contentLayout.div(R"(
        <vertical margin=30
            <weight=35 titleLabel>
            <weight=25>
            <weight=28 row1 <weight=130 lbl1><val1>>
            <weight=8>
            <weight=28 row2 <weight=130 lbl2><val2>>
            <weight=8>
            <weight=28 row3 <weight=130 lbl3><val3>>
            <weight=8>
            <weight=28 row4 <weight=130 lbl4><val4>>
            <weight=8>
            <weight=28 row5 <weight=130 lbl5><val5>>
            <>
        >
    )");
    
    profileTitle = std::make_unique<nana::label>(contentPanel, "User Profile");
    profileLbl1 = std::make_unique<nana::label>(contentPanel, "Username:");
    profileVal1 = std::make_unique<nana::label>(contentPanel, user->getUsername());
    profileLbl2 = std::make_unique<nana::label>(contentPanel, "Full Name:");
    profileVal2 = std::make_unique<nana::label>(contentPanel, user->getFullName());
    profileLbl3 = std::make_unique<nana::label>(contentPanel, "Email:");
    profileVal3 = std::make_unique<nana::label>(contentPanel, user->getEmail());
    profileLbl4 = std::make_unique<nana::label>(contentPanel, "Phone:");
    profileVal4 = std::make_unique<nana::label>(contentPanel, user->getPhone());
    profileLbl5 = std::make_unique<nana::label>(contentPanel, "User ID:");
    profileVal5 = std::make_unique<nana::label>(contentPanel, std::to_string(user->getUserId()));
    
    profileTitle->bgcolor(styles::colors::surface);
    profileTitle->fgcolor(styles::colors::accent);
    profileTitle->typeface(nana::paint::font("Segoe UI", 16, {600}));
    
    auto styleLbl = [](nana::label& l) {
        l.bgcolor(styles::colors::surface);
        l.fgcolor(styles::colors::textSecondary);
    };
    auto styleVal = [](nana::label& l) {
        l.bgcolor(styles::colors::surface);
        l.fgcolor(styles::colors::textPrimary);
        l.typeface(nana::paint::font("Segoe UI", 12, {600}));
    };
    
    styleLbl(*profileLbl1); styleLbl(*profileLbl2); styleLbl(*profileLbl3); 
    styleLbl(*profileLbl4); styleLbl(*profileLbl5);
    styleVal(*profileVal1); styleVal(*profileVal2); styleVal(*profileVal3); 
    styleVal(*profileVal4); styleVal(*profileVal5);
    
    contentLayout["titleLabel"] << *profileTitle;
    contentLayout["lbl1"] << *profileLbl1; contentLayout["val1"] << *profileVal1;
    contentLayout["lbl2"] << *profileLbl2; contentLayout["val2"] << *profileVal2;
    contentLayout["lbl3"] << *profileLbl3; contentLayout["val3"] << *profileVal3;
    contentLayout["lbl4"] << *profileLbl4; contentLayout["val4"] << *profileVal4;
    contentLayout["lbl5"] << *profileLbl5; contentLayout["val5"] << *profileVal5;
    contentLayout.collocate();
    
    statusLabel.caption("Profile: " + user->getFullName());
}

void MainWindow::showLoginDialog() {
    nana::form dlg{*this, nana::API::make_center(*this, 380, 300)};
    dlg.caption("Login");
    dlg.bgcolor(styles::colors::background);
    
    nana::place layout{dlg};
    
    nana::label title{dlg, "Welcome Back!"};
    title.bgcolor(styles::colors::background);
    title.fgcolor(styles::colors::textPrimary);
    title.typeface(nana::paint::font("Segoe UI", 18, {700}));
    title.text_align(nana::align::center);
    
    nana::label lblU{dlg, "Username:"};
    lblU.bgcolor(styles::colors::background);
    lblU.fgcolor(styles::colors::textSecondary);
    nana::textbox txtU{dlg};
    
    nana::label lblP{dlg, "Password:"};
    lblP.bgcolor(styles::colors::background);
    lblP.fgcolor(styles::colors::textSecondary);
    nana::textbox txtP{dlg};
    txtP.mask('*');
    
    nana::button btnL{dlg, "Login"};
    styles::applyPrimaryButton(btnL);
    
    nana::button btnR{dlg, "Register"};
    styles::applySecondaryButton(btnR);
    
    nana::label status{dlg};
    status.bgcolor(styles::colors::background);
    status.fgcolor(styles::colors::error);
    
    layout.div(R"(
        <vertical margin=20
            <weight=40 title>
            <weight=12>
            <weight=20 lblU><weight=32 txtU>
            <weight=8>
            <weight=20 lblP><weight=32 txtP>
            <weight=15>
            <weight=35 btnL>
            <weight=8>
            <weight=35 btnR>
            <weight=10>
            <weight=20 status>
        >
    )");
    
    layout["title"] << title;
    layout["lblU"] << lblU; layout["txtU"] << txtU;
    layout["lblP"] << lblP; layout["txtP"] << txtP;
    layout["btnL"] << btnL;
    layout["btnR"] << btnR;
    layout["status"] << status;
    layout.collocate();
    
    bool doRegister = false;
    
    btnL.events().click([this, &dlg, &txtU, &txtP, &status] {
        if (bookingSystem.login(txtU.caption(), txtP.caption())) {
            dlg.close();
            btnLogin.caption("Logout");
            updateUserDisplay();
            statusLabel.caption("Welcome, " + bookingSystem.getCurrentUser()->getFullName() + "!");
        } else {
            status.caption("Invalid credentials");
        }
    });
    
    btnR.events().click([&dlg, &doRegister] {
        doRegister = true;
        dlg.close();
    });
    
    dlg.modality();
    
    if (doRegister) showRegisterDialog();
}

void MainWindow::showRegisterDialog() {
    nana::form dlg{*this, nana::API::make_center(*this, 380, 420)};
    dlg.caption("Register");
    dlg.bgcolor(styles::colors::background);
    
    nana::place layout{dlg};
    
    nana::label title{dlg, "Create Account"};
    title.bgcolor(styles::colors::background);
    title.fgcolor(styles::colors::textPrimary);
    title.typeface(nana::paint::font("Segoe UI", 18, {700}));
    title.text_align(nana::align::center);
    
    nana::label lblU{dlg, "Username:"};
    nana::label lblP{dlg, "Password:"};
    nana::label lblE{dlg, "Email:"};
    nana::label lblN{dlg, "Full Name:"};
    nana::label lblPh{dlg, "Phone:"};
    
    for (auto* l : {&lblU, &lblP, &lblE, &lblN, &lblPh}) {
        l->bgcolor(styles::colors::background);
        l->fgcolor(styles::colors::textSecondary);
    }
    
    nana::textbox txtU{dlg}, txtP{dlg}, txtE{dlg}, txtN{dlg}, txtPh{dlg};
    txtP.mask('*');
    
    nana::button btnR{dlg, "Create Account"};
    styles::applyAccentButton(btnR);
    
    nana::label status{dlg};
    status.bgcolor(styles::colors::background);
    status.fgcolor(styles::colors::error);
    
    layout.div(R"(
        <vertical margin=18
            <weight=35 title>
            <weight=8>
            <weight=18 lblU><weight=28 txtU>
            <weight=5>
            <weight=18 lblP><weight=28 txtP>
            <weight=5>
            <weight=18 lblE><weight=28 txtE>
            <weight=5>
            <weight=18 lblN><weight=28 txtN>
            <weight=5>
            <weight=18 lblPh><weight=28 txtPh>
            <weight=12>
            <weight=38 btnR>
            <weight=8>
            <weight=18 status>
        >
    )");
    
    layout["title"] << title;
    layout["lblU"] << lblU; layout["txtU"] << txtU;
    layout["lblP"] << lblP; layout["txtP"] << txtP;
    layout["lblE"] << lblE; layout["txtE"] << txtE;
    layout["lblN"] << lblN; layout["txtN"] << txtN;
    layout["lblPh"] << lblPh; layout["txtPh"] << txtPh;
    layout["btnR"] << btnR;
    layout["status"] << status;
    layout.collocate();
    
    btnR.events().click([this, &dlg, &txtU, &txtP, &txtE, &txtN, &txtPh, &status] {
        if (bookingSystem.registerUser(txtU.caption(), txtP.caption(),
                                       txtE.caption(), txtN.caption(), txtPh.caption())) {
            dlg.close();
            showMessage("Success", "Account created! You can now login.");
        } else {
            status.caption("Registration failed");
        }
    });
    
    dlg.modality();
}

void MainWindow::updateUserDisplay() {
    if (bookingSystem.isLoggedIn()) {
        userLabel.caption("User: " + bookingSystem.getCurrentUser()->getUsername());
        btnLogin.caption("Logout");
    } else {
        userLabel.caption("Not logged in");
        btnLogin.caption("Login");
    }
}

void MainWindow::showMessage(const std::string& title, const std::string& message) {
    nana::msgbox mb(*this, title);
    mb << message;
    mb.show();
}

void MainWindow::run() {
    show();
    nana::exec();
}
