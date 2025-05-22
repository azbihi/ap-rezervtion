#include "Flight.h"
#include <sstream>
#include <iomanip>

static int next_flight_id = 1;

Flight::Flight(const std::string& flight_number, const std::string& origin,
               const std::string& destination, time_t departure_time,
               int available_seats, double ticket_price) {
    this->flight_id = next_flight_id++;
    this->flight_number = flight_number;
    this->origin = origin;
    this->destination = destination;
    this->departure_time = departure_time;
    this->available_seats = available_seats;
    this->ticket_price = ticket_price;
    this->is_deleted = false;
}

bool Flight::reserveSeat() {
    if (available_seats > 0) {
        available_seats--;
        return true;
    }
    return false;
}

bool Flight::cancelSeat() {
    available_seats++;
    return true;
}

std::string Flight::toCSV() const {
    std::stringstream ss;
    ss << flight_id << ","
       << flight_number << ","
       << origin << ","
       << destination << ","
       << departure_time << ","
       << available_seats << ","
       << std::fixed << std::setprecision(2) << ticket_price << ","
       << (is_deleted ? "1" : "0");
    return ss.str();
}

Flight Flight::fromCSV(const std::string& csv_line) {
    std::stringstream ss(csv_line);
    std::string token;
    
    // Read fields
    std::getline(ss, token, ',');
    int id = std::stoi(token);
    
    std::string flight_num, orig, dest;
    std::getline(ss, flight_num, ',');
    std::getline(ss, orig, ',');
    std::getline(ss, dest, ',');
    
    std::getline(ss, token, ',');
    time_t dep_time = std::stoll(token);
    
    std::getline(ss, token, ',');
    int seats = std::stoi(token);
    
    std::getline(ss, token, ',');
    double price = std::stod(token);
    
    std::getline(ss, token, ',');
    bool deleted = (token == "1");
    
    // Create flight
    Flight f(flight_num, orig, dest, dep_time, seats, price);
    f.flight_id = id;
    f.is_deleted = deleted;
    
    if (id >= next_flight_id) {
        next_flight_id = id + 1;
    }
    
    return f;
}
