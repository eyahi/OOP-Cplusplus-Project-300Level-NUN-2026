/**
 * @file User.h
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief User class for user management
 * 
 * This file defines the User class which handles user authentication
 * and profile management in the travel booking system.
 */

#ifndef USER_H
#define USER_H

#include <string>

/**
 * @class User
 * @brief Represents a user in the booking system
 * 
 * Manages user credentials, profile information, and authentication.
 * Demonstrates encapsulation with private members and public interface.
 */
class User {
private:
    int userId;             ///< Unique user identifier
    std::string username;   ///< Login username
    std::string password;   ///< Hashed password (simplified for demo)
    std::string email;      ///< User's email address
    std::string fullName;   ///< User's full name
    std::string phone;      ///< Contact phone number

public:
    /**
     * @brief Construct a new User object
     * @param userId Unique user ID
     * @param username Login username
     * @param password User password
     * @param email Email address
     * @param fullName Full name
     * @param phone Phone number
     */
    User(int userId, const std::string& username, const std::string& password,
         const std::string& email, const std::string& fullName,
         const std::string& phone);

    /**
     * @brief Default constructor
     */
    User();

    // Getters
    int getUserId() const;
    std::string getUsername() const;
    std::string getEmail() const;
    std::string getFullName() const;
    std::string getPhone() const;

    // Setters
    void setEmail(const std::string& email);
    void setFullName(const std::string& fullName);
    void setPhone(const std::string& phone);
    void setPassword(const std::string& password);

    /**
     * @brief Authenticate user with given password
     * @param inputPassword Password to verify
     * @return true if password matches, false otherwise
     */
    bool authenticate(const std::string& inputPassword) const;

    /**
     * @brief Display user profile information
     */
    void displayProfile() const;

    /**
     * @brief Get password for file storage (simplified - in real app, would be hashed)
     * @return Password string
     */
    std::string getPassword() const;
};

#endif // USER_H
