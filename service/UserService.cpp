// UserService.cpp
#include "UserService.h"
#include <sqlite3.h>
#include <iostream>

#include "../data/User.h"
#include "../database/DatabaseManager.h"

// Implement proper password hashing (this is just a placeholder)
std::string UserService::hashPassword(const std::string& password) {
    // In a real application, use a proper hashing algorithm like bcrypt
    // This is just for demonstration - NEVER use this in production!
    return std::to_string(std::hash<std::string>{}(password));
}


std::optional<User> UserService::createUser(const std::string &username,
                                            const std::string &password,
                                            const std::string &role) {
    if (usernameExists(username)) {
        return std::nullopt;
    }

    const std::string sql = "INSERT INTO users (username, password_hash, role) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbManager.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    std::string hashedPassword = hashPassword(password);

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, hashedPassword.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, role.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    int userId = sqlite3_last_insert_rowid(dbManager.getConnection());
    sqlite3_finalize(stmt);

    std::optional<User> user;
    user.emplace(userId, username, hashedPassword, role, std::time(nullptr));

    return user;
}

std::optional<User> UserService::getUser(int userId) {
    const std::string sql = "SELECT id, username, password_hash, role, created_at FROM users WHERE id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbManager.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_int(stmt, 1, userId);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    User user(
        sqlite3_column_int(stmt, 0),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
        sqlite3_column_int64(stmt, 4)
    );
    sqlite3_finalize(stmt);
    std::optional<User> temp;
    temp.emplace(user);
    return temp;
}

std::optional<User> UserService::authenticate(const std::string& username,
                                            const std::string& password) {
    auto user = getUserByUsername(username);
    if (!user) {
        return std::nullopt;
    }

    std::string hashedInput = hashPassword(password);
    if (bool(user->getPasswordHash() == hashedInput)) {
        return user;
    }
    return std::nullopt;
}
bool UserService::usernameExists(const std::string& username) {
    const std::string sql = "SELECT 1 FROM users WHERE username = ? LIMIT 1";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbManager.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    bool exists = sqlite3_step(stmt) == SQLITE_ROW;
    sqlite3_finalize(stmt);
    return exists;
}

std::optional<User> UserService::getUserByUsername(const std::string& username) {
    const std::string sql = "SELECT id, username, password_hash, role, created_at FROM users WHERE username = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbManager.getConnection(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    User user(
        sqlite3_column_int(stmt, 0),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
        sqlite3_column_int64(stmt, 4)
    );
    std::optional<User> temp;
    temp.emplace(user);
    sqlite3_finalize(stmt);
    return temp;
}

