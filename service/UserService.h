#pragma once

#include <optional>
#include <vector>
#include"../database/DatabaseManager.h"
#include "../model/User.h"

class UserService {
private:
    DatabaseManager& dbManager;

    // Password hashing (implement this properly in .cpp)
    std::string hashPassword(const std::string& password);

public:
    explicit UserService(DatabaseManager& dbManager) : dbManager(dbManager) {}

    // CRUD Operations
    std::optional<User> createUser(const std::string &username,
                                   const std::string &password,
                                   const std::string &role = "user");

    std::optional<User> getUser(int userId);
    std::optional<User> getUserByUsername(const std::string& username);
    std::vector<User> getAllUsers();

    bool updateUser(User& user);
    bool deleteUser(int userId);

    // Authentication
    std::optional<User> authenticate(const std::string& username,
                                   const std::string& password);

    // Password management
    bool changePassword(int userId, const std::string& newPassword);

    // Utility
    bool usernameExists(const std::string& username);
};
