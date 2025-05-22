#pragma once
#include <string>
#include <ctime>

class Flight {
private:
    int flight_id;
    std::string flight_number;
    std::string origin;
    std::string destination;
    time_t departure_time;
    int available_seats;
    double ticket_price;
    bool is_deleted;

public:
    Flight(const std::string& flight_number, const std::string& origin,
           const std::string& destination, time_t departure_time,
           int available_seats, double ticket_price);

    // Getters
    int getFlightId() const { return flight_id; }
    std::string getFlightNumber() const { return flight_number; }
    std::string getOrigin() const { return origin; }
    std::string getDestination() const { return destination; }
    time_t getDepartureTime() const { return departure_time; }
    int getAvailableSeats() const { return available_seats; }
    double getTicketPrice() const { return ticket_price; }
    bool isDeleted() const { return is_deleted; }

    // Operations
    bool reserveSeat();
    bool cancelSeat();
    void softDelete() { is_deleted = true; }

    // For file operations
    std::string toCSV() const;
    static Flight fromCSV(const std::string& csv_line);
};
