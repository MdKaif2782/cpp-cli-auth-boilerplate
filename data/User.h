// User.h
#pragma once
#include <string>
#include <ctime>

class User {
private:
    int id;
    std::string username;
    std::string passwordHash;
    std::string role;
    std::time_t createdAt;

public:
    User(int id = -1, 
         std::string username = "", 
         std::string passwordHash = "", 
         std::string role = "user",
         std::time_t createdAt = 0);

    // Getters
    int getId() const;
    const std::string& getUsername() const;
    const std::string& getPasswordHash() const;
    const std::string& getRole() const;
    std::time_t getCreatedAt() const;
    
    // Other methods
    std::string getCreationDate() const;
    bool isAdmin() const;
};