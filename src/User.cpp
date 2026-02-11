/**
 * @file User.cpp
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Implementation of User class
 */

#include "User.h"
#include <iostream>
#include <iomanip>

User::User(int userId, const std::string& username, const std::string& password,
           const std::string& email, const std::string& fullName,
           const std::string& phone)
    : userId(userId), username(username), password(password),
      email(email), fullName(fullName), phone(phone) {}

User::User() : userId(0), username(""), password(""), email(""), fullName(""), phone("") {}

int User::getUserId() const {
    return userId;
}

std::string User::getUsername() const {
    return username;
}

std::string User::getEmail() const {
    return email;
}

std::string User::getFullName() const {
    return fullName;
}

std::string User::getPhone() const {
    return phone;
}

std::string User::getPassword() const {
    return password;
}

void User::setEmail(const std::string& newEmail) {
    email = newEmail;
}

void User::setFullName(const std::string& newFullName) {
    fullName = newFullName;
}

void User::setPhone(const std::string& newPhone) {
    phone = newPhone;
}

void User::setPassword(const std::string& newPassword) {
    password = newPassword;
}

bool User::authenticate(const std::string& inputPassword) const {
    return password == inputPassword;
}

void User::displayProfile() const {
    std::cout << "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                      👤 USER PROFILE                         ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║ User ID: " << std::setw(52) << std::left << userId << "║\n";
    std::cout << "║ Username: " << std::setw(51) << username << "║\n";
    std::cout << "║ Full Name: " << std::setw(50) << fullName << "║\n";
    std::cout << "║ Email: " << std::setw(54) << email << "║\n";
    std::cout << "║ Phone: " << std::setw(54) << phone << "║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════╝\n";
}
