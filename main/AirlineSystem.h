#ifndef AIRLINESYSTEM_H
#define AIRLINESYSTEM_H

#include "Passenger.h"
#include "Flight.h"
#include "Reservation.h"

#include <vector>
#include <string>

class AirlineSystem {
private:
    std::vector<Passenger> passengers;
    std::vector<Flight> flights;
    std::vector<Reservation> reservations;

    int nextPassengerId;
    int nextFlightId;
    int nextReservationId;

//    void loadPassengers();
//    void savePassengers();
//    void loadFlights();
//    void saveFlights();
//    void loadReservations();
//    void saveReservations();

public:
    AirlineSystem();

    void addPassenger(const std::string& name, const std::string& passport, const std::string& nationalCode, const std::string& nationality, double wallet);
    Passenger* findPassengerById(int id);
    void editPassenger(int id, const std::string& newPassport, double newWallet);
    void listPassengers();

    void addFlight(const std::string& number, const std::string& origin, const std::string& destination, const std::string& datetime, int seats, double price);
    Flight* findFlightById(int id);
    void listFlights();

    void makeReservation(int passengerId, int flightId);
    void listReservations();
    Reservation* findReservationById(int id);
    void cancelReservation(int reservationId);

//    void saveAll();
};

#endif
