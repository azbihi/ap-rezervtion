#include "Reservation.h"
#include <sstream>
#include <ctime>
#include <stdexcept>
static int next_reservation_id = 1;

// Define FlightCompletedException
class FlightCompletedException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Flight has already been completed. Refund not possible.";
    }
};

// Define RefundNotAllowedException
class RefundNotAllowedException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Refund is not allowed less than 24 hours before departure.";
    }
};

Reservation::Reservation(int passenger_id, int flight_id, double amount_paid) {
    this->reservation_id = next_reservation_id++;
    this->passenger_id = passenger_id;
    this->flight_id = flight_id;
    this->amount_paid = amount_paid;
    this->reservation_time = std::time(nullptr);
    this->flight_departure_time = 0;  // Will be set later
    this->is_cancelled = false;
    this->is_deleted = false;
}

double Reservation::calculateRefundAmount(time_t current_time) const {
    if (current_time > flight_departure_time) {
        throw FlightCompletedException();
    }

    time_t time_diff = flight_departure_time - current_time;
    const int HOURS_48 = 48 * 60 * 60;
    const int HOURS_24 = 24 * 60 * 60;
    
    if (time_diff > HOURS_48) {
        return amount_paid * 0.9; // 90% refund if more than 48 hours
    } else if (time_diff > HOURS_24) {
        return amount_paid * 0.5; // 50% refund if between 24 and 48 hours
    }
    
    throw RefundNotAllowedException(); // Less than 24 hours, no refund
}

std::string Reservation::toCSV() const {
    std::stringstream ss;
    ss << reservation_id << ","
       << passenger_id << ","
       << flight_id << ","
       << amount_paid << ","
       << reservation_time << ","
       << flight_departure_time << ","
       << (is_cancelled ? "1" : "0") << ","
       << (is_deleted ? "1" : "0");
    return ss.str();
}

Reservation Reservation::fromCSV(const std::string& csv_line) {
    std::stringstream ss(csv_line);
    std::string token;
    
    std::getline(ss, token, ',');
    int res_id = std::stoi(token);
    
    std::getline(ss, token, ',');
    int pass_id = std::stoi(token);
    
    std::getline(ss, token, ',');
    int fl_id = std::stoi(token);
    
    std::getline(ss, token, ',');
    double amount = std::stod(token);
    
    Reservation r(pass_id, fl_id, amount);
    r.reservation_id = res_id;
    
    std::getline(ss, token, ',');
    r.reservation_time = std::stoll(token);
    
    std::getline(ss, token, ',');
    r.flight_departure_time = std::stoll(token);
    
    std::getline(ss, token, ',');
    r.is_cancelled = (token == "1");
    
    std::getline(ss, token, ',');
    r.is_deleted = (token == "1");
    
    if (res_id >= next_reservation_id) {
        next_reservation_id = res_id + 1;
    }
    
    return r;
}
