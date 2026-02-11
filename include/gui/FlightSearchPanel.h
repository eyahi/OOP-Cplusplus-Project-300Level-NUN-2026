/**
 * @file FlightSearchPanel.h
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Flight search and booking panel
 */

#ifndef FLIGHTSEARCHPANEL_H
#define FLIGHTSEARCHPANEL_H

#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/spinbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/place.hpp>

#include "BookingSystem.h"
#include <functional>

/**
 * @class FlightSearchPanel
 * @brief Panel for searching and booking flights
 */
class FlightSearchPanel : public nana::panel<true> {
private:
    BookingSystem& bookingSystem;
    nana::place layout{*this};
    
    // Search section
    nana::group searchGroup{*this, "Search Flights"};
    nana::label lblOrigin{searchGroup, "From:"};
    nana::textbox txtOrigin{searchGroup};
    nana::label lblDestination{searchGroup, "To:"};
    nana::textbox txtDestination{searchGroup};
    nana::button btnSearch{searchGroup, "Search"};
    
    // Results section
    nana::group resultsGroup{*this, "Available Flights"};
    nana::listbox flightList{resultsGroup};
    
    // Booking section
    nana::group bookingGroup{*this, "Book Flight"};
    nana::label lblPassengers{bookingGroup, "Passengers:"};
    nana::spinbox spnPassengers{bookingGroup};
    nana::label lblClass{bookingGroup, "Class:"};
    nana::combox cmbClass{bookingGroup};
    nana::label lblName{bookingGroup, "Passenger Name:"};
    nana::textbox txtPassengerName{bookingGroup};
    nana::button btnBook{bookingGroup, "Book Now"};
    
    // Status
    nana::label lblStatus{*this};
    
    // Callbacks
    std::function<void(const std::string&)> onFlightBooked;
    
    void setupLayout();
    void setupStyling();
    void setupEvents();
    void populateFlightList(const std::vector<Flight*>& flights);
    
public:
    FlightSearchPanel(nana::window parent, BookingSystem& system);
    
    void setOnFlightBooked(std::function<void(const std::string&)> callback);
    void refresh();
};

#endif // FLIGHTSEARCHPANEL_H
