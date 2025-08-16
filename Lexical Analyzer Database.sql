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
VALUES ('Admin', 'Tamim', 'admin@tamim.com', '123456');


SELECT id, first_name, last_name, email, password FROM users;
SELECT * FROM users;
SELECT * FROM analysis;


SET FOREIGN_KEY_CHECKS = 0;
DELETE FROM analysis;
DELETE FROM users;

DELETE FROM users WHERE id = 5;

SET FOREIGN_KEY_CHECKS = 1;
