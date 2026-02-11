/**
 * @file Bookable.cpp
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Implementation of Bookable abstract class
 */

#include "Bookable.h"

Bookable::Bookable(int id, const std::string& name, double price, const std::string& location)
    : id(id), name(name), price(price), location(location) {}

int Bookable::getId() const {
    return id;
}

std::string Bookable::getName() const {
    return name;
}

double Bookable::getPrice() const {
    return price;
}

std::string Bookable::getLocation() const {
    return location;
}

void Bookable::setPrice(double newPrice) {
    if (newPrice >= 0) {
        price = newPrice;
    }
}
