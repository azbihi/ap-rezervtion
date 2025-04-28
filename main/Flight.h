#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>

class Flight {
private:
    int flight_id;
    std::string flight_number;
    std::string origin;
    std::string destination;
    std::string datetime;
    int available_seats;
    double price;
    bool is_deleted;

public:
    Flight();
    Flight(int id, const std::string& number, const std::string& origin, const std::string& destination, const std::string& datetime, int seats, double price);

    int getFlightId() const;
    std::string getFlightNumber() const;
    std::string getOrigin() const;
    std::string getDestination() const;
    std::string getDatetime() const;
    int getAvailableSeats() const;
    double getPrice() const;
    bool isDeleted() const;

    void setAvailableSeats(int seats);
    void setPrice(double price);
    void markDeleted();

    void decreaseSeat();

};

#endif
