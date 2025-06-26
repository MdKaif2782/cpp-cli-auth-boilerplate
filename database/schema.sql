-- schema.sql
PRAGMA foreign_keys = ON;

-- Users table for authentication
CREATE TABLE IF NOT EXISTS users (
                                     id INTEGER PRIMARY KEY AUTOINCREMENT,
                                     username TEXT UNIQUE NOT NULL,
                                     password_hash TEXT NOT NULL,  -- Store hashed passwords only!
                                     role TEXT NOT NULL DEFAULT 'user',  -- 'admin' or 'user'
                                     created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Crimes table
CREATE TABLE IF NOT EXISTS crimes (
                                      id INTEGER PRIMARY KEY AUTOINCREMENT,
                                      user_id INTEGER NOT NULL,
                                      title TEXT NOT NULL,
                                      description TEXT,
                                      severity INTEGER CHECK (severity BETWEEN 1 AND 10),
    reported_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    is_solved BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
    );

-- Indexes for better performance
CREATE INDEX IF NOT EXISTS idx_crimes_user_id ON crimes(user_id);
CREATE INDEX IF NOT EXISTS idx_crimes_is_solved ON crimes(is_solved);