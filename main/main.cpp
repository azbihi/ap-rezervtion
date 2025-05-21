#include <iostream>
#include "AirlineSystem.h"
#include <csignal>

AirlineSystem* globalSystem = nullptr;

void signalHandler(int signum) {
    if (globalSystem) {
        globalSystem->saveAll();
    }
    std::cout << "\nData saved. Exiting due to signal " << signum << ".\n";
    exit(signum);
}

int main() {
    AirlineSystem system;
    globalSystem = &system;

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    int choice;

    while (true) {
        std::cout << "\n===== Airline Reservation System =====\n";
        std::cout << "1. Add Passenger\n";
        std::cout << "2. Add Flight\n";
        std::cout << "3. Make Reservation\n";
        std::cout << "4. List Passengers\n";
        std::cout << "5. List Flights\n";
        std::cout << "6. List Reservations\n";
        std::cout << "7. Cancel Reservation\n";
        std::cout << "8. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string name, passport, nationalCode, nationality;
            double wallet;
            std::cout << "Enter full name: ";
            std::cin.ignore();
            getline(std::cin, name);
            std::cout << "Enter passport number: ";
            getline(std::cin, passport);
            std::cout << "Enter national code: ";
            getline(std::cin, nationalCode);
            std::cout << "Enter nationality: ";
            getline(std::cin, nationality);
            std::cout << "Enter wallet balance: ";
            std::cin >> wallet;
            system.addPassenger(name, passport, nationalCode, nationality, wallet);
        }
        else if (choice == 2) {
            std::string flightNumber, origin, destination, datetime;
            int seats;
            double price;
            std::cout << "Enter flight number: ";
            std::cin.ignore();
            getline(std::cin, flightNumber);
            std::cout << "Enter origin: ";
            getline(std::cin, origin);
            std::cout << "Enter destination: ";
            getline(std::cin, destination);
            std::cout << "Enter date and time (e.g., 2025-05-20 14:00): ";
            getline(std::cin, datetime);
            std::cout << "Enter available seats: ";
            std::cin >> seats;
            std::cout << "Enter ticket price: ";
            std::cin >> price;
            system.addFlight(flightNumber, origin, destination, datetime, seats, price);
        }
        else if (choice == 3) {
            int passengerId, flightId;
            std::cout << "Enter passenger ID: ";
            std::cin >> passengerId;
            std::cout << "Enter flight ID: ";
            std::cin >> flightId;
            system.makeReservation(passengerId, flightId);
        }
        else if (choice == 4) {
            system.listPassengers();
        }
        else if (choice == 5) {
            system.listFlights();
        }
        else if (choice == 6) {
            system.listReservations();
        }
        else if (choice == 7) {
            int reservationId;
            std::cout << "Enter reservation ID to cancel: ";
            std::cin >> reservationId;
            system.cancelReservation(reservationId);
        }
        else if (choice == 8) {
            system.saveAll();
            std::cout << "Goodbye!\n";
            break;
        }
        else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }

    system.saveAll();
    return 0;
}
