CREATE DATABASE lexical_db;
USE lexical_db;

CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    first_name VARCHAR(50),
    last_name VARCHAR(50),
    email VARCHAR(100) UNIQUE,
    password VARCHAR(255)
);

CREATE TABLE analysis (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT,
    input_code TEXT,
    tokens TEXT,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id)
);
INSERT INTO users (first_name, last_name, email, password)
VALUES (
    'Admin',
    'Tamim',
    'admin@tamim.com',
    '$pbkdf2-sha256$29000$CXn/0RfK2UakmSkLRpWtdA$hATYpM6zmOskkG2iZlyxj/XoAlO8AKimlId6bWY3xks'
);

SELECT email, password FROM users;
SELECT email, plain_password FROM users;
SELECT first_name, last_name, email, plain_password FROM users;

SET SQL_SAFE_UPDATES = 0;

DELETE FROM analysis;
DELETE FROM users;

