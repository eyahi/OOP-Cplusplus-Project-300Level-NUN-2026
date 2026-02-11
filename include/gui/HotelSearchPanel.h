/**
 * @file HotelSearchPanel.h
 * @author [Your Name]
 * @date 2026-01-21
 * @brief Hotel search and booking panel
 */

#ifndef HOTELSEARCHPANEL_H
#define HOTELSEARCHPANEL_H

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
 * @class HotelSearchPanel
 * @brief Panel for searching and booking hotels
 */
class HotelSearchPanel : public nana::panel<true> {
private:
    BookingSystem& bookingSystem;
    nana::place layout{*this};
    
    // Search section
    nana::group searchGroup{*this, "Search Hotels"};
    nana::label lblCity{searchGroup, "City:"};
    nana::textbox txtCity{searchGroup};
    nana::label lblStars{searchGroup, "Min Stars:"};
    nana::combox cmbStars{searchGroup};
    nana::button btnSearch{searchGroup, "Search"};
    
    // Destination suggestion
    nana::label lblSuggestion{*this};
    nana::button btnShowDestination{*this, "Show Hotels at Destination"};
    
    // Results section
    nana::group resultsGroup{*this, "Available Hotels"};
    nana::listbox hotelList{resultsGroup};
    
    // Booking section
    nana::group bookingGroup{*this, "Book Hotel"};
    nana::label lblCheckIn{bookingGroup, "Check-in:"};
    nana::textbox txtCheckIn{bookingGroup};
    nana::label lblCheckOut{bookingGroup, "Check-out:"};
    nana::textbox txtCheckOut{bookingGroup};
    nana::label lblRooms{bookingGroup, "Rooms:"};
    nana::spinbox spnRooms{bookingGroup};
    nana::label lblGuests{bookingGroup, "Guests:"};
    nana::spinbox spnGuests{bookingGroup};
    nana::label lblGuestName{bookingGroup, "Guest Name:"};
    nana::textbox txtGuestName{bookingGroup};
    nana::button btnBook{bookingGroup, "Book Now"};
    
    // Status
    nana::label lblStatus{*this};
    
    void setupLayout();
    void setupStyling();
    void setupEvents();
    void populateHotelList(const std::vector<Hotel*>& hotels);
    void updateDestinationSuggestion();
    
public:
    HotelSearchPanel(nana::window parent, BookingSystem& system);
    
    void refresh();
    void showHotelsInCity(const std::string& city);
};

#endif // HOTELSEARCHPANEL_H
