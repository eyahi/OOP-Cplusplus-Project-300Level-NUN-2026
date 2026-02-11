/**
 * @file Booking.cpp
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Implementation of Booking abstract class
 */

#include "Booking.h"

Booking::Booking(int bookingId, int userId, const std::string& bookingDate, double totalPrice)
    : bookingId(bookingId), userId(userId), bookingDate(bookingDate),
      totalPrice(totalPrice), status(BookingStatus::PENDING) {}

int Booking::getBookingId() const {
    return bookingId;
}

int Booking::getUserId() const {
    return userId;
}

std::string Booking::getBookingDate() const {
    return bookingDate;
}

double Booking::getTotalPrice() const {
    return totalPrice;
}

BookingStatus Booking::getStatus() const {
    return status;
}

std::string Booking::getStatusString() const {
    switch (status) {
        case BookingStatus::PENDING:
            return "PENDING";
        case BookingStatus::CONFIRMED:
            return "CONFIRMED";
        case BookingStatus::CANCELLED:
            return "CANCELLED";
        case BookingStatus::COMPLETED:
            return "COMPLETED";
        default:
            return "UNKNOWN";
    }
}

void Booking::confirm() {
    if (status == BookingStatus::PENDING) {
        status = BookingStatus::CONFIRMED;
    }
}

void Booking::cancel() {
    if (status != BookingStatus::COMPLETED && status != BookingStatus::CANCELLED) {
        status = BookingStatus::CANCELLED;
    }
}
