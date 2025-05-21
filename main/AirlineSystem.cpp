#include "AirlineSystem.h"
#include <fstream>
#include <sstream>
#include <iostream>

AirlineSystem::AirlineSystem() : nextPassengerId(1), nextFlightId(1), nextReservationId(1) {
    loadPassengers();
    loadFlights();
    loadReservations();
}

// Passenger Management

void AirlineSystem::addPassenger(const std::string& name, const std::string& passport, const std::string& nationalCode, const std::string& nationality, double wallet) {
    Passenger p(nextPassengerId++, name, passport, nationalCode, nationality, wallet);
    passengers.push_back(p);
//    savePassengers();
}

Passenger* AirlineSystem::findPassengerById(int id) {
    for (auto& p : passengers) {
        if (p.getPassengerId() == id && !p.isDeleted()) {
            return &p;
        }
    }
    return nullptr;
}

void AirlineSystem::editPassenger(int id, const std::string& newPassport, double newWallet) {
    Passenger* p = findPassengerById(id);
    if (p) {
        p->setPassportNumber(newPassport);
        p->setWalletBalance(newWallet);
//        savePassengers();
    }
}

void AirlineSystem::listPassengers() {
    for (const auto& p : passengers) {
        if (!p.isDeleted()) {
            std::cout << p.getPassengerId() << " - " << p.getFullName() << " - " << p.getWalletBalance() << " Toman\n";
        }
    }
}

// Flight Management

void AirlineSystem::addFlight(const std::string& number, const std::string& origin, const std::string& destination, const std::string& datetime, int seats, double price) {
    Flight f(nextFlightId++, number, origin, destination, datetime, seats, price);
    flights.push_back(f);
//    saveFlights();
}

Flight* AirlineSystem::findFlightById(int id) {
    for (auto& f : flights) {
        if (f.getFlightId() == id && !f.isDeleted()) {
            return &f;
        }
    }
    return nullptr;
}

void AirlineSystem::listFlights() {
    for (const auto& f : flights) {
        if (!f.isDeleted()) {
            std::cout << f.getFlightId() << " - " << f.getFlightNumber() << " from " << f.getOrigin() << " to " << f.getDestination() << " [" << f.getAvailableSeats() << " seats]\n";
        }
    }
}

// Reservation Management

void AirlineSystem::makeReservation(int passengerId, int flightId) {
    Passenger* p = findPassengerById(passengerId);
    Flight* f = findFlightById(flightId);

    if (!p) {
        std::cout << "Passenger not found!\n";
        return;
    }
    if (!f) {
        std::cout << "Flight not found!\n";
        return;
    }
    if (f->getAvailableSeats() <= 0) {
        std::cout << "Flight is full!\n";
        return;
    }
    if (p->getWalletBalance() < f->getPrice()) {
        std::cout << "Insufficient balance!\n";
        return;
    }

    f->decreaseSeat();
    p->setWalletBalance(p->getWalletBalance() - f->getPrice());

    Reservation r(nextReservationId++, passengerId, flightId, f->getPrice());
    reservations.push_back(r);

//    saveFlights();
//    savePassengers();
//    saveReservations();
}

void AirlineSystem::listReservations() {
    for (const auto& r : reservations) {
        std::cout << r.getReservationId() << " - Passenger ID: " << r.getPassengerId() << " - Flight ID: " << r.getFlightId() << "\n";
    }
}

Reservation* AirlineSystem::findReservationById(int id) {
    for (auto& r : reservations) {
        if (r.getReservationId() == id && !r.isCanceled()) {
            return &r;
        }
    }
    return nullptr;
}

void AirlineSystem::cancelReservation(int reservationId) {
    Reservation* r = findReservationById(reservationId);
    if (r) {
        r->cancel();
//        saveReservations();
    }
}

// File Operations

void AirlineSystem::loadPassengers() {
    std::ifstream file("passengers.csv");
    if (!file.is_open()) return;
    std::string line;
    while (getline(file, line)) {
        Passenger p = Passenger::fromCSV(line);
        passengers.push_back(p);
        if (p.getPassengerId() >= nextPassengerId) {
            nextPassengerId = p.getPassengerId() + 1;
        }
    }
    file.close();
}

void AirlineSystem::savePassengers() {
    std::ofstream file("passengers.csv");
    for (const auto& p : passengers) {
        file << p.toCSV() << "\n";
    }
    file.close();
}

void AirlineSystem::loadFlights() {
    std::ifstream file("flights.csv");
    if (!file.is_open()) return;
    std::string line;
    while (getline(file, line)) {
        Flight f = Flight::fromCSV(line);
        flights.push_back(f);
        if (f.getFlightId() >= nextFlightId) {
            nextFlightId = f.getFlightId() + 1;
        }
    }
    file.close();
}

void AirlineSystem::saveFlights() {
    std::ofstream file("flights.csv");
    for (const auto& f : flights) {
        file << f.toCSV() << "\n";
    }
    file.close();
}

void AirlineSystem::loadReservations() {
    std::ifstream file("reservations.csv");
    if (!file.is_open()) return;
    std::string line;
    while (getline(file, line)) {
        Reservation r = Reservation::fromCSV(line);
        reservations.push_back(r);
        if (r.getReservationId() >= nextReservationId) {
            nextReservationId = r.getReservationId() + 1;
        }
    }
    file.close();
}

void AirlineSystem::saveReservations() {
    std::ofstream file("reservations.csv");
    for (const auto& r : reservations) {
        file << r.toCSV() << "\n";
    }
    file.close();
}

void AirlineSystem::saveAll() {
    savePassengers();
    saveFlights();
    saveReservations();
}
