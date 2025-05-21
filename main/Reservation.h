#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

class Reservation {
private:
    int reservation_id;
    int passenger_id;
    int flight_id;
    double amount_paid;
    bool is_canceled;

public:
    Reservation();
    Reservation(int id, int passengerId, int flightId, double amount);

    int getReservationId() const;
    int getPassengerId() const;
    int getFlightId() const;
    double getAmountPaid() const;
    bool isCanceled() const;

    void cancel();

    static Reservation fromCSV(const std::string& line);
    std::string toCSV() const;
};

#endif
