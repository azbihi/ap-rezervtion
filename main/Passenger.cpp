#include "Passenger.h"
#include <sstream>
#include <iostream>

static int next_passenger_id = 1;

Passenger::Passenger(const std::string& name, const std::string& passport_number,
                   const std::string& national_id, const std::string& nationality) {
    this->passenger_id = next_passenger_id++;
    this->name = name;
    this->passport_number = passport_number;
    this->national_id = national_id;
    this->nationality = nationality;
    this->wallet_balance = 0.0;
    this->is_deleted = false;
}

std::string Passenger::toCSV() const {
    std::stringstream ss;
    ss << passenger_id << "," 
       << name << "," 
       << passport_number << "," 
       << national_id << "," 
       << nationality << "," 
       << wallet_balance << "," 
       << (is_deleted ? "1" : "0");
    return ss.str();
}

Passenger Passenger::fromCSV(const std::string& csv_line) {
    std::stringstream ss(csv_line);
    std::string token;
    
    // Read fields
    std::getline(ss, token, ',');
    int id = std::stoi(token);
    
    std::string name, passport, national_id, nationality;
    double balance;
    bool deleted;
    
    std::getline(ss, name, ',');
    std::getline(ss, passport, ',');
    std::getline(ss, national_id, ',');
    std::getline(ss, nationality, ',');
    
    std::getline(ss, token, ',');
    balance = std::stod(token);
    
    std::getline(ss, token, ',');
    deleted = (token == "1");
    
    // Create passenger
    Passenger p(name, passport, national_id, nationality);
    p.passenger_id = id;
    p.wallet_balance = balance;
    p.is_deleted = deleted;
    
    // Update next_passenger_id if necessary
    if (id >= next_passenger_id) {
        next_passenger_id = id + 1;
    }
    
    return p;
}
