// User.cpp
#include "User.h"

#include <iomanip>
#include <sstream>

// Constructor implementation
User::User(int id,
           std::string username,
           std::string passwordHash,
           std::string role,
           std::time_t createdAt)
    : id(id),
      username(std::move(username)),
      passwordHash(std::move(passwordHash)),
      role(std::move(role)),
      createdAt(createdAt) {}

// Getter implementations
int User::getId() const { return id; }
const std::string& User::getUsername() const { return username; }
const std::string& User::getPasswordHash() const { return passwordHash; }
const std::string& User::getRole() const { return role; }
std::time_t User::getCreatedAt() const { return createdAt; }

// Helper method implementations
bool User::isAdmin() const {
    return role == "admin";
}

std::string User::getCreationDate() const {
    std::tm* ptm = std::localtime(&createdAt);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ptm);
    return buffer;
}