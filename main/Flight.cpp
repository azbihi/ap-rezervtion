#include "Flight.h"
#include <sstream>

Flight::Flight() : flight_id(0), available_seats(0), price(0.0), is_deleted(false) {}

Flight::Flight(int id, const std::string& number, const std::string& origin, const std::string& destination, const std::string& datetime, int seats, double price)
        : flight_id(id), flight_number(number), origin(origin), destination(destination), datetime(datetime), available_seats(seats), price(price), is_deleted(false) {}

int Flight::getFlightId() const { return flight_id; }
std::string Flight::getFlightNumber() const { return flight_number; }
std::string Flight::getOrigin() const { return origin; }
std::string Flight::getDestination() const { return destination; }
std::string Flight::getDatetime() const { return datetime; }
int Flight::getAvailableSeats() const { return available_seats; }
double Flight::getPrice() const { return price; }
bool Flight::isDeleted() const { return is_deleted; }

void Flight::setAvailableSeats(int seats) { available_seats = seats; }
void Flight::setPrice(double price1) { this->price = price1; }
void Flight::markDeleted() { is_deleted = true; }

void Flight::decreaseSeat() {
    if (available_seats > 0) available_seats--;
}

Flight Flight::fromCSV(const std::string& line) {
    std::stringstream ss(line);
    std::string token;
    Flight f;
    getline(ss, token, ','); f.flight_id = std::stoi(token);
    getline(ss, f.flight_number, ',');
    getline(ss, f.origin, ',');
    getline(ss, f.destination, ',');
    getline(ss, f.datetime, ',');
    getline(ss, token, ','); f.available_seats = std::stoi(token);
    getline(ss, token, ','); f.price = std::stod(token);
    getline(ss, token, ','); f.is_deleted = (token == "1");
    return f;
}

std::string Flight::toCSV() const {
    std::stringstream ss;
    ss << flight_id << "," << flight_number << "," << origin << "," << destination << "," << datetime << "," << available_seats << "," << price << "," << (is_deleted ? "1" : "0");
    return ss.str();
}

