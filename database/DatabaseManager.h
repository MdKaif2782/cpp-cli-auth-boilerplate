// DatabaseManager.h
#pragma once
#include <sqlite3.h>
#include <memory>
#include <string>
#include <fstream>
#include <filesystem>  // For filesystem operations

class DatabaseManager {
private:
    static std::unique_ptr<DatabaseManager> instance;
    sqlite3* db;
    std::string dbPath;

    DatabaseManager(const std::string& dbPath, bool createIfMissing);
    bool initializeDatabase(const std::string& schemaPath = "");

public:
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // Factory method with more explicit control
    static DatabaseManager& getInstance(const std::string& dbPath = "database/app.db",
                                      bool createIfMissing = true,
                                      const std::string& schemaPath = "database/schema.sql");

    sqlite3* getConnection() const { return db; }
    const std::string& getDatabasePath() const { return dbPath; }

    bool execute(const std::string& sql);
    bool executeWithParams(const std::string& sql,
                         const std::vector<std::string>& params);

    bool tableExists(const std::string& tableName);
    bool backupDatabase(const std::string& backupPath);
};