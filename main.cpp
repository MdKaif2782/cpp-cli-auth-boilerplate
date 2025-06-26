#include <iostream>

#include "database/DatabaseManager.h"
using namespace std;
int main() {
    // Create new database or open existing
    auto& db = DatabaseManager::getInstance("/Users/mdkaifibnzaman/CLionProjects/cpp-cli-auth-boilerplate/database/crime_lite.db", true, "/Users/mdkaifibnzaman/CLionProjects/cpp-cli-auth-boilerplate/database/schema.sql");

    // Open existing database (fail if doesn't exist)
    try {
        auto& db = DatabaseManager::getInstance("existing.db", false);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Check if a table exists
    if (db.tableExists("users")) {
        std::cout << "Users table exists\n";
    }
    return 0;
}
