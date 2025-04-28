#include "Flight.h"


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

