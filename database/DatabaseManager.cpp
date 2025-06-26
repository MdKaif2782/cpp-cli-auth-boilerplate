#include "DatabaseManager.h"
#include <iostream>
#include <sstream>

std::unique_ptr<DatabaseManager> DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager(const std::string& dbPath, bool createIfMissing)
    : dbPath(dbPath) {

    if (!createIfMissing) {
        if (!std::filesystem::exists(dbPath)) {
            throw std::runtime_error("Database file does not exist and createIfMissing is false");
        }
    }

    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (sqlite3_open_v2(dbPath.c_str(), &db, flags, nullptr) != SQLITE_OK) {
        std::string errMsg = sqlite3_errmsg(db);
        sqlite3_close(db);
        throw std::runtime_error("Failed to open database: " + errMsg);
    }

    if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "Warning: Could not enable foreign key constraints\n";
    }

    sqlite3_busy_timeout(db, 5000);
}

bool DatabaseManager::initializeDatabase(const std::string& schemaPath) {
    if (schemaPath.empty()) {
        return true;
    }

    if (tableExists("users")) {
        return true;
    }

    std::ifstream file(schemaPath);
    if (!file.is_open()) {
        std::cerr << "Could not open schema file: " << schemaPath << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string schemaSql = buffer.str();

    char* errMsg = nullptr;
    if (sqlite3_exec(db, schemaSql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Schema initialization error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    std::cout << "Database initialized with schema from: " << schemaPath << std::endl;
    return true;
}

DatabaseManager::~DatabaseManager() {
    if (db) {
        // Finalize any remaining statements
        sqlite3_stmt* stmt;
        while ((stmt = sqlite3_next_stmt(db, nullptr)) != nullptr) {
            sqlite3_finalize(stmt);
        }

        // Close the database
        if (sqlite3_close(db) != SQLITE_OK) {
            std::cerr << "Warning: Failed to close database properly\n";
        }
    }
}

DatabaseManager& DatabaseManager::getInstance(const std::string& dbPath,
                                           bool createIfMissing,
                                           const std::string& schemaPath) {
    if (!instance) {
        instance = std::unique_ptr<DatabaseManager>(
            new DatabaseManager(dbPath, createIfMissing));

        // Initialize schema if requested
        if (!schemaPath.empty()) {
            if (!instance->initializeDatabase(schemaPath)) {
                throw std::runtime_error("Failed to initialize database schema");
            }
        }
    }
    return *instance;
}

bool DatabaseManager::tableExists(const std::string& tableName) {
    const std::string sql = "SELECT 1 FROM sqlite_master WHERE type='table' AND name=?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, tableName.c_str(), -1, SQLITE_TRANSIENT);
    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);

    return exists;
}
