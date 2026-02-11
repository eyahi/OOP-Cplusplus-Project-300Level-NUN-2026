/**
 * @file MainWindow.h
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Main application window for Travel Booking System
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/place.hpp>

#include "BookingSystem.h"
#include <memory>

/**
 * @class MainWindow
 * @brief Main application window with premium styling
 */
class MainWindow : public nana::form {
private:
    BookingSystem& bookingSystem;
    
    // Layout manager
    nana::place layout{*this};
    
    // Header section
    nana::label titleLabel{*this, "TravelBook"};
    nana::label subtitleLabel{*this, "Your Premium Travel Companion"};
    
    // Navigation buttons
    nana::button btnFlights{*this, "Flights"};
    nana::button btnHotels{*this, "Hotels"};
    nana::button btnMyBookings{*this, "My Bookings"};
    nana::button btnProfile{*this, "Profile"};
    nana::button btnLogin{*this, "Login"};
    
    // Main content panel
    nana::panel<true> contentPanel{*this};
    nana::place contentLayout{contentPanel};
    
    // Status bar
    nana::label statusLabel{*this, "Welcome to TravelBook!"};
    
    // Current user display
    nana::label userLabel{*this, "Not logged in"};
    
    // ===== Flight View Widgets =====
    std::unique_ptr<nana::label> flightLblFrom;
    std::unique_ptr<nana::textbox> flightTxtFrom;
    std::unique_ptr<nana::label> flightLblTo;
    std::unique_ptr<nana::textbox> flightTxtTo;
    std::unique_ptr<nana::button> flightBtnSearch;
    std::unique_ptr<nana::label> flightResultsLabel;
    std::unique_ptr<nana::listbox> flightList;
    std::unique_ptr<nana::label> flightLblPassengers;
    std::unique_ptr<nana::spinbox> flightSpnPassengers;
    std::unique_ptr<nana::label> flightLblClass;
    std::unique_ptr<nana::combox> flightCmbClass;
    std::unique_ptr<nana::label> flightLblName;
    std::unique_ptr<nana::textbox> flightTxtName;
    std::unique_ptr<nana::button> flightBtnBook;
    
    // ===== Hotel View Widgets =====
    std::unique_ptr<nana::label> hotelLblSuggestion;
    std::unique_ptr<nana::label> hotelLblCity;
    std::unique_ptr<nana::textbox> hotelTxtCity;
    std::unique_ptr<nana::label> hotelLblStars;
    std::unique_ptr<nana::combox> hotelCmbStars;
    std::unique_ptr<nana::button> hotelBtnSearch;
    std::unique_ptr<nana::label> hotelResultsLabel;
    std::unique_ptr<nana::listbox> hotelList;
    std::unique_ptr<nana::label> hotelLblCheckIn;
    std::unique_ptr<nana::textbox> hotelTxtCheckIn;
    std::unique_ptr<nana::label> hotelLblCheckOut;
    std::unique_ptr<nana::textbox> hotelTxtCheckOut;
    std::unique_ptr<nana::label> hotelLblRooms;
    std::unique_ptr<nana::spinbox> hotelSpnRooms;
    std::unique_ptr<nana::label> hotelLblGuests;
    std::unique_ptr<nana::spinbox> hotelSpnGuests;
    std::unique_ptr<nana::label> hotelLblGuestName;
    std::unique_ptr<nana::textbox> hotelTxtGuestName;
    std::unique_ptr<nana::button> hotelBtnBook;
    
    // ===== Bookings View Widgets =====
    std::unique_ptr<nana::label> bookingsTitle;
    std::unique_ptr<nana::listbox> bookingsList;
    std::unique_ptr<nana::button> bookingsBtnCancel;
    
    // ===== Profile View Widgets =====
    std::unique_ptr<nana::label> profileTitle;
    std::unique_ptr<nana::label> profileLbl1, profileVal1;
    std::unique_ptr<nana::label> profileLbl2, profileVal2;
    std::unique_ptr<nana::label> profileLbl3, profileVal3;
    std::unique_ptr<nana::label> profileLbl4, profileVal4;
    std::unique_ptr<nana::label> profileLbl5, profileVal5;
    
    // Styling helpers
    void applyPremiumStyling();
    void setupLayout();
    void setupEventHandlers();
    
    // View functions
    void showFlightsView();
    void showHotelsView();
    void showBookingsView();
    void showProfileView();
    void showLoginDialog();
    void showRegisterDialog();
    
    // Clear all view widgets
    void clearViewWidgets();
    
    // Helper functions
    void updateUserDisplay();
    void showMessage(const std::string& title, const std::string& message);
    void searchFlights();
    void bookSelectedFlight();
    void searchHotels();
    void bookSelectedHotel();
    void cancelSelectedBooking();
    void updateNavButtons(int active);
    
public:
    MainWindow(BookingSystem& system);
    void run();
};

#endif // MAINWINDOW_H
