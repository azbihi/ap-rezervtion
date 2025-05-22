#pragma once
#include <string>
#include <ctime>

class Reservation {
private:
    int reservation_id;
    int passenger_id;
    int flight_id;
    double amount_paid;
    time_t reservation_time;
    time_t flight_departure_time; // Add this field
    bool is_cancelled;
    bool is_deleted;

public:
    Reservation(int passenger_id, int flight_id, double amount_paid);

    // Getters
    int getReservationId() const { return reservation_id; }
    int getPassengerId() const { return passenger_id; }
    int getFlightId() const { return flight_id; }
    double getAmountPaid() const { return amount_paid; }
    time_t getReservationTime() const { return reservation_time; }
    time_t getFlightDepartureTime() const { return flight_departure_time; } // Add this method
    bool isCancelled() const { return is_cancelled; }
    bool isDeleted() const { return is_deleted; }

    // Setters
    void setFlightDepartureTime(time_t time) { flight_departure_time = time; } // Add this method

    // Operations
    double calculateRefundAmount(time_t current_time) const;
    void cancel() { is_cancelled = true; }
    void softDelete() { is_deleted = true; }

    // For file operations
    std::string toCSV() const;
    static Reservation fromCSV(const std::string& csv_line);
};
