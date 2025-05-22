#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <algorithm>
#include "AirlineExceptions.h"

void FileManager::ensureDirectoryExists() {
    std::filesystem::create_directories("data");
}

std::vector<Passenger> FileManager::loadPassengers() {
    std::vector<Passenger> passengers;
    ensureDirectoryExists();
    
    std::ifstream file("data/" + PASSENGERS_FILE);
    if (!file.is_open()) {
        // Create new file if it doesn't exist
        std::ofstream newFile("data/" + PASSENGERS_FILE);
        if (!newFile.is_open()) {
            throw AirlineException("Could not create passengers file");
        }
        newFile.close();
        return passengers;
    }

    std::string line;
    while (std::getline(file, line)) {
        try {
            if (!line.empty()) {
                passengers.push_back(Passenger::fromCSV(line));
            }
        } catch (const std::exception& e) {
            std::cerr << "Warning: Skipping invalid passenger data: " << e.what() << std::endl;
        }
    }
    
    file.close();
    return passengers;
}

std::vector<Flight> FileManager::loadFlights() {
    std::vector<Flight> flights;
    ensureDirectoryExists();
    
    std::ifstream file("data/" + FLIGHTS_FILE);
    if (!file.is_open()) {
        // Create new file if it doesn't exist
        std::ofstream newFile("data/" + FLIGHTS_FILE);
        if (!newFile.is_open()) {
            throw AirlineException("Could not create flights file");
        }
        newFile.close();
        return flights;
    }

    try {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                flights.push_back(Flight::fromCSV(line));
            }
        }
    } catch (const std::exception& e) {
        throw AirlineException("Error reading flights file: " + std::string(e.what()));
    }
    
    file.close();
    return flights;
}

std::vector<Reservation> FileManager::loadReservations() {
    std::vector<Reservation> reservations;
    ensureDirectoryExists();
    
    std::ifstream file("data/" + RESERVATIONS_FILE);
    if (!file.is_open()) {
        // Create new file if it doesn't exist
        std::ofstream newFile("data/" + RESERVATIONS_FILE);
        if (!newFile.is_open()) {
            throw AirlineException("Could not create reservations file");
        }
        newFile.close();
        return reservations;
    }

    try {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                reservations.push_back(Reservation::fromCSV(line));
            }
        }
    } catch (const std::exception& e) {
        throw AirlineException("Error reading reservations file: " + std::string(e.what()));
    }
    
    file.close();
    return reservations;
}

void FileManager::validatePassengerData(const Passenger& passenger) {
    if (passenger.getName().empty()) {
        throw InvalidInputException("name");
    }
    if (passenger.getPassportNumber().empty()) {
        throw InvalidInputException("passport number");
    }
    if (passenger.getNationalId().empty()) {
        throw InvalidInputException("national ID");
    }
}

void FileManager::validateFlightData(const Flight& flight) {
    if (flight.getFlightNumber().empty()) {
        throw InvalidInputException("flight number");
    }
    if (flight.getOrigin().empty() || flight.getDestination().empty()) {
        throw InvalidInputException("origin/destination");
    }
    if (flight.getAvailableSeats() < 0) {
        throw InvalidInputException("available seats");
    }
    if (flight.getTicketPrice() <= 0) {
        throw InvalidInputException("ticket price");
    }
}

void FileManager::validateReservationData(const Reservation& reservation) {
    if (reservation.getPassengerId() <= 0) {
        throw InvalidInputException("passenger ID");
    }
    if (reservation.getFlightId() <= 0) {
        throw InvalidInputException("flight ID");
    }
    if (reservation.getAmountPaid() <= 0) {
        throw InvalidInputException("amount paid");
    }
}

void FileManager::savePassengers(const std::vector<Passenger>& passengers) {
    ensureDirectoryExists();
    std::ofstream file("data/" + PASSENGERS_FILE);
    if (!file.is_open()) {
        throw AirlineException("Could not open passengers file for writing");
    }
    
    for (const auto& passenger : passengers) {
        try {
            validatePassengerData(passenger);
            file << passenger.toCSV() << std::endl;
            if (file.fail()) {
                throw AirlineException("Error writing passenger data");
            }
        } catch (const std::exception& e) {
            file.close();
            throw AirlineException("Failed to save passenger: " + std::string(e.what()));
        }
    }
    
    file.close();
}

void FileManager::saveFlights(const std::vector<Flight>& flights) {
    ensureDirectoryExists();
    std::ofstream file("data/" + FLIGHTS_FILE);
    if (!file.is_open()) {
        throw AirlineException("Could not open flights file for writing");
    }
    
    for (const auto& flight : flights) {
        try {
            validateFlightData(flight);
            file << flight.toCSV() << std::endl;
            if (file.fail()) {
                throw AirlineException("Error writing flight data");
            }
        } catch (const std::exception& e) {
            file.close();
            throw AirlineException("Failed to save flight: " + std::string(e.what()));
        }
    }
    
    file.close();
}

void FileManager::saveReservations(const std::vector<Reservation>& reservations) {
    ensureDirectoryExists();
    std::ofstream file("data/" + RESERVATIONS_FILE);
    if (!file.is_open()) {
        throw AirlineException("Could not open reservations file for writing");
    }
    
    for (const auto& reservation : reservations) {
        try {
            validateReservationData(reservation);
            file << reservation.toCSV() << std::endl;
            if (file.fail()) {
                throw AirlineException("Error writing reservation data");
            }
        } catch (const std::exception& e) {
            file.close();
            throw AirlineException("Failed to save reservation: " + std::string(e.what()));
        }
    }
    
    file.close();
}

void FileManager::generateReport(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw AirlineException("Could not create report file");
    }
    
    file << content;
    file.close();
}

void FileManager::generatePassengerReport(const std::string& filename, const Passenger& passenger, 
                                        const std::vector<Reservation>& reservations) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw AirlineException("Could not create passenger report file");
    }
    
    try {
        file << std::fixed << std::setprecision(2);
        file << "Passenger Report\n"
             << "----------------\n"
             << "ID: " << passenger.getPassengerId() << "\n"
             << "Name: " << passenger.getName() << "\n"
             << "Passport: " << passenger.getPassportNumber() << "\n"
             << "Balance: " << passenger.getWalletBalance() << "\n\n"
             << "Reservations:\n";
             
        for (const auto& res : reservations) {
            if (res.getPassengerId() == passenger.getPassengerId()) {
                file << "Reservation ID: " << res.getReservationId() 
                     << ", Amount: " << res.getAmountPaid()
                     << ", Status: " << (res.isCancelled() ? "Cancelled" : "Active")
                     << "\n";
            }
        }
        
        file.close();
    } catch (const std::exception& e) {
        file.close();
        throw AirlineException("Error generating passenger report: " + std::string(e.what()));
    }
}

void FileManager::generateFlightReport(const std::string& filename, const Flight& flight, 
                                     const std::vector<Reservation>& reservations,
                                     const std::vector<Passenger>& passengers) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw AirlineException("Could not create flight report file");
    }
    
    try {
        file << std::fixed << std::setprecision(2);
        file << "Flight Report\n"
             << "-------------\n"
             << "Flight ID: " << flight.getFlightId() << "\n"
             << "Number: " << flight.getFlightNumber() << "\n"
             << "From: " << flight.getOrigin() << " To: " << flight.getDestination() << "\n"
             << "Available Seats: " << flight.getAvailableSeats() << "\n\n"
             << "Passengers:\n";
         
        for (const auto& res : reservations) {
            if (res.getFlightId() == flight.getFlightId() && !res.isCancelled()) {
                auto passenger = std::find_if(passengers.begin(), passengers.end(),
                    [&](const Passenger& p) { return p.getPassengerId() == res.getPassengerId(); });
            
                if (passenger != passengers.end()) {
                    file << "Passenger: " << passenger->getName() 
                         << " (ID: " << passenger->getPassengerId() << ")\n";
                }
            }
        }
        
        file.close();
    } catch (const std::exception& e) {
        file.close();
        throw AirlineException("Error generating flight report: " + std::string(e.what()));
    }
}

void FileManager::generateReservationsReport(const std::string& filename, 
                                           const std::vector<Reservation>& reservations,
                                           const std::vector<Passenger>& passengers,
                                           const std::vector<Flight>& flights) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw AirlineException("Could not create reservations report file");
    }
    
    try {
        file << std::fixed << std::setprecision(2);
        file << "Reservations Report\n"
             << "------------------\n\n";
         
        for (const auto& res : reservations) {
            auto passenger = std::find_if(passengers.begin(), passengers.end(),
                [&](const Passenger& p) { return p.getPassengerId() == res.getPassengerId(); });
            
            auto flight = std::find_if(flights.begin(), flights.end(),
                [&](const Flight& f) { return f.getFlightId() == res.getFlightId(); });
            
            if (passenger != passengers.end() && flight != flights.end()) {
                file << "Reservation ID: " << res.getReservationId() << "\n"
                     << "Passenger: " << passenger->getName() << "\n"
                     << "Flight: " << flight->getFlightNumber() << "\n"
                     << "Amount: " << res.getAmountPaid() << "\n"
                     << "Status: " << (res.isCancelled() ? "Cancelled" : "Active") << "\n\n";
            }
        }
        
        file.close();
    } catch (const std::exception& e) {
        file.close();
        throw AirlineException("Error generating reservations report: " + std::string(e.what()));
    }
}
