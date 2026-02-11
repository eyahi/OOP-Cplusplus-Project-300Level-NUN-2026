/**
 * @file Bookable.h
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Abstract base class for all bookable items (flights, hotels)
 * 
 * This file defines the Bookable abstract class which serves as the
 * base class for Flight and Hotel classes, demonstrating inheritance
 * and polymorphism in OOP.
 */

#ifndef BOOKABLE_H
#define BOOKABLE_H

#include <string>
#include <iostream>

/**
 * @class Bookable
 * @brief Abstract base class representing any bookable travel item
 * 
 * This class provides a common interface for all bookable items in the
 * travel booking system. It uses pure virtual functions to enforce
 * implementation in derived classes.
 */
class Bookable {
protected:
    int id;                 ///< Unique identifier for the bookable item
    std::string name;       ///< Name/title of the bookable item
    double price;           ///< Base price of the item
    std::string location;   ///< Location associated with the item

public:
    /**
     * @brief Construct a new Bookable object
     * @param id Unique identifier
     * @param name Name of the item
     * @param price Base price
     * @param location Associated location
     */
    Bookable(int id, const std::string& name, double price, const std::string& location);

    /**
     * @brief Virtual destructor for proper cleanup in derived classes
     */
    virtual ~Bookable() = default;

    // Getters
    int getId() const;
    std::string getName() const;
    double getPrice() const;
    std::string getLocation() const;

    // Setters
    void setPrice(double price);

    /**
     * @brief Display detailed information about the bookable item
     * Pure virtual function - must be implemented by derived classes
     */
    virtual void displayInfo() const = 0;

    /**
     * @brief Get the type of bookable item
     * @return String representing the type ("Flight", "Hotel", etc.)
     */
    virtual std::string getType() const = 0;

    /**
     * @brief Check if the item is available for booking
     * @return true if available, false otherwise
     */
    virtual bool isAvailable() const = 0;
};

#endif // BOOKABLE_H
