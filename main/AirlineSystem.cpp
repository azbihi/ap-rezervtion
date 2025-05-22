#include "AirlineSystem.h"
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>  // Add this
#include <ctime>    // Add this

AirlineSystem::AirlineSystem() {
    loadAllData();
    ensureFileExists();
}

AirlineSystem::~AirlineSystem() {
    saveAllData();
}

void AirlineSystem::loadAllData() {
    try {
        passengers = file_manager.loadPassengers();
        flights = file_manager.loadFlights();
        reservations = file_manager.loadReservations();
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load data: " + std::string(e.what()));
    }
}

void AirlineSystem::saveAllData() {
    try {
        file_manager.savePassengers(passengers);
        file_manager.saveFlights(flights);
        file_manager.saveReservations(reservations);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to save data: " + std::string(e.what()));
    }
}

void AirlineSystem::ensureFileExists() {
    try {
        if (passengers.empty()) {
            file_manager.savePassengers(passengers);
        }
        if (flights.empty()) {
            file_manager.saveFlights(flights);
        }
        if (reservations.empty()) {
            file_manager.saveReservations(reservations);
        }
    } catch (const std::exception& e) {
        throw AirlineException("Failed to create initial files: " + std::string(e.what()));
    }
}

void AirlineSystem::autoSave() {
    if (data_changed) {
        try {
            saveAllData();
            data_changed = false;
        } catch (const std::exception& e) {
            // Log error but don't throw to prevent disrupting normal operation
            std::cerr << "Auto-save failed: " << e.what() << std::endl;
        }
    }
}

bool AirlineSystem::isNationalIdTaken(const std::string& national_id, int exclude_id) {
    return std::find_if(passengers.begin(), passengers.end(),
        [&](const Passenger& p) {
            return !p.isDeleted() && p.getNationalId() == national_id && 
                   p.getPassengerId() != exclude_id;
        }) != passengers.end();
}

bool AirlineSystem::isPassportNumberTaken(const std::string& passport, int exclude_id) {
    return std::find_if(passengers.begin(), passengers.end(),
        [&](const Passenger& p) {
            return !p.isDeleted() && p.getPassportNumber() == passport && 
                   p.getPassengerId() != exclude_id;
        }) != passengers.end();
}

int AirlineSystem::addPassenger(const Passenger& passenger) {
    try {
        if (isNationalIdTaken(passenger.getNationalId())) {
            throw AirlineException("National ID already exists");
        }
        if (isPassportNumberTaken(passenger.getPassportNumber())) {
            throw AirlineException("Passport number already exists");
        }
        
        passengers.push_back(passenger);
        markDataAsChanged();
        autoSave();
        return passenger.getPassengerId();
    } catch (const std::exception& e) {
        throw AirlineException("Failed to add passenger: " + std::string(e.what()));
    }
}

Passenger* AirlineSystem::findPassenger(int passenger_id) {
    auto it = std::find_if(passengers.begin(), passengers.end(),
        [passenger_id](const Passenger& p) { 
            return p.getPassengerId() == passenger_id && !p.isDeleted(); 
        });
    return it != passengers.end() ? &(*it) : nullptr;
}

std::vector<Passenger> AirlineSystem::searchPassengers(const std::string& search_term) {
    std::vector<Passenger> results;
    for (const auto& p : passengers) {
        if (p.isDeleted()) continue;
        
        if (p.getName().find(search_term) != std::string::npos ||
            p.getPassportNumber().find(search_term) != std::string::npos ||
            p.getNationalId().find(search_term) != std::string::npos) {
            results.push_back(p);
        }
    }
    return results;
}

int AirlineSystem::addFlight(const Flight& flight) {
    try {
        flights.push_back(flight);
        markDataAsChanged();
        autoSave();
        return flight.getFlightId();
    } catch (const std::exception& e) {
        throw AirlineException("Failed to add flight: " + std::string(e.what()));
    }
}

Flight* AirlineSystem::findFlight(int flight_id) {
    auto it = std::find_if(flights.begin(), flights.end(),
        [flight_id](const Flight& f) { 
            return f.getFlightId() == flight_id && !f.isDeleted(); 
        });
    return it != flights.end() ? &(*it) : nullptr;
}

std::vector<Flight> AirlineSystem::searchFlights(const std::string& search_term) {
    std::vector<Flight> results;
    for (const auto& f : flights) {
        if (f.isDeleted()) continue;
        
        if (f.getFlightNumber().find(search_term) != std::string::npos ||
            f.getOrigin().find(search_term) != std::string::npos ||
            f.getDestination().find(search_term) != std::string::npos) {
            results.push_back(f);
        }
    }
    return results;
}

Reservation* AirlineSystem::findReservation(int reservation_id) {
    auto it = std::find_if(reservations.begin(), reservations.end(),
        [reservation_id](const Reservation& r) { 
            return r.getReservationId() == reservation_id && !r.isDeleted(); 
        });
    return it != reservations.end() ? &(*it) : nullptr;
}

void AirlineSystem::validateReservation(int passenger_id, int flight_id) {
    auto passenger = findPassenger(passenger_id);
    if (!passenger) {
        throw PassengerNotFoundException();
    }
    
    auto flight = findFlight(flight_id);
    if (!flight) {
        throw FlightNotFoundException();
    }
    
    if (flight->getAvailableSeats() <= 0) {
        throw AirlineException("Flight is full");
    }
    
    if (passenger->getWalletBalance() < flight->getTicketPrice()) {
        throw InsufficientBalanceException();
    }
}

int AirlineSystem::makeReservation(int passenger_id, int flight_id) {
    try {
        validateReservation(passenger_id, flight_id);
        auto passenger = findPassenger(passenger_id);
        auto flight = findFlight(flight_id);
        
        passenger->updateWalletBalance(-flight->getTicketPrice());
        flight->reserveSeat();
        
        Reservation reservation(passenger_id, flight_id, flight->getTicketPrice());
        reservation.setFlightDepartureTime(flight->getDepartureTime()); // Set departure time
        reservations.push_back(reservation);
        
        markDataAsChanged();
        autoSave();
        
        return reservation.getReservationId();
    } catch (const std::exception& e) {
        throw AirlineException("Failed to make reservation: " + std::string(e.what()));
    }
}

bool AirlineSystem::cancelReservation(int reservation_id) {
    auto reservation = findReservation(reservation_id);
    if (!reservation) {
        throw ReservationNotFoundException();
    }
    
    if (reservation->isCancelled()) {
        throw AirlineException("Reservation is already cancelled");
    }

    auto flight = findFlight(reservation->getFlightId());
    if (!flight) {
        throw FlightNotFoundException();
    }

    auto passenger = findPassenger(reservation->getPassengerId());
    if (!passenger) {
        throw PassengerNotFoundException();
    }

    try {
        double refund = reservation->calculateRefundAmount(std::time(nullptr));
        passenger->updateWalletBalance(refund);
        flight->cancelSeat();
        reservation->cancel();
        
        markDataAsChanged();
        autoSave();
        
        return true;
    } catch (const FlightCompletedException& e) {
        throw;  // Re-throw
    } catch (const RefundNotAllowedException& e) {
        throw;  // Re-throw
    } catch (const std::exception& e) {
        throw AirlineException("Error during cancellation: " + std::string(e.what()));
    }
}

void AirlineSystem::generateFlightReport(int flight_id) {
    auto flight = findFlight(flight_id);
    if (!flight) throw std::runtime_error("Flight not found");
    
    std::stringstream report;
    std::time_t t = flight->getDepartureTime();
    char time_str[26];
    ctime_s(time_str, sizeof(time_str), &t);

    report << "Flight Report\n";
    report << "Flight ID: " << flight->getFlightId() << "\n";
    report << "Flight Number: " << flight->getFlightNumber() << "\n";
    report << "From: " << flight->getOrigin() << " To: " << flight->getDestination() << "\n";
    report << "Departure Time: " << time_str;  // ctime_s adds a newline
    report << "Available Seats: " << flight->getAvailableSeats() << "\n";
    report << "Ticket Price: $" << std::fixed << std::setprecision(2) << flight->getTicketPrice() << "\n";
    
    file_manager.generateReport("flight_report_" + std::to_string(flight_id) + ".txt", 
                              report.str());
}

void AirlineSystem::generatePassengerReport(int passenger_id) {
    auto passenger = findPassenger(passenger_id);
    if (!passenger) {
        throw PassengerNotFoundException();
    }
    
    std::string filename = "passenger_report_" + std::to_string(passenger_id) + ".txt";
    file_manager.generatePassengerReport(filename, *passenger, reservations);
}

void AirlineSystem::generateReservationReport() {
    std::string filename = "reservations_report.txt";
    file_manager.generateReservationsReport(filename, reservations, passengers, flights);
}

void AirlineSystem::displayPassengerDetails(const Passenger& passenger) {
    std::cout << "\nPassenger Details:\n"
              << "----------------\n"
              << "ID: " << passenger.getPassengerId() << "\n"
              << "Name: " << passenger.getName() << "\n"
              << "Passport Number: " << passenger.getPassportNumber() << "\n"
              << "National ID: " << passenger.getNationalId() << "\n"
              << "Nationality: " << passenger.getNationality() << "\n"
              << "Wallet Balance: " << std::fixed << std::setprecision(2) 
              << passenger.getWalletBalance() << "\n";
}

void AirlineSystem::listAllPassengers() {
    std::cout << "\nAll Passengers:\n"
              << "--------------\n";
    for (const auto& passenger : passengers) {
        if (!passenger.isDeleted()) {
            displayPassengerDetails(passenger);
            std::cout << "--------------\n";
        }
    }
}

void AirlineSystem::displayFlightDetails(const Flight& flight) {
    std::time_t t = flight.getDepartureTime();
    std::cout << "\nFlight Details:\n"
              << "----------------\n"
              << "ID: " << flight.getFlightId() << "\n"
              << "Flight Number: " << flight.getFlightNumber() << "\n"
              << "From: " << flight.getOrigin() << "\n"
              << "To: " << flight.getDestination() << "\n"
              << "Departure Time: " << std::ctime(&t)
              << "Available Seats: " << flight.getAvailableSeats() << "\n"
              << "Ticket Price: $" << std::fixed << std::setprecision(2) 
              << flight.getTicketPrice() << "\n";
}

void AirlineSystem::listAllFlights() {
    std::cout << "\nAll Flights:\n"
              << "--------------\n";
    for (const auto& flight : flights) {
        if (!flight.isDeleted()) {
            displayFlightDetails(flight);
            std::cout << "--------------\n";
        }
    }
}

void AirlineSystem::listAllReservations() {
    std::cout << "\nAll Reservations:\n"
              << "-----------------\n";
    bool found = false;
    
    for (const auto& res : reservations) {
        if (res.isDeleted()) continue;
        
        auto passenger = findPassenger(res.getPassengerId());
        auto flight = findFlight(res.getFlightId());
        
        if (passenger && flight) {
            found = true;
            std::cout << "Reservation ID: " << res.getReservationId() << "\n"
                     << "Passenger: " << passenger->getName() 
                     << " (ID: " << passenger->getPassengerId() << ")\n"
                     << "Flight: " << flight->getFlightNumber() 
                     << " (" << flight->getOrigin() << " -> " << flight->getDestination() << ")\n"
                     << "Amount Paid: $" << std::fixed << std::setprecision(2) 
                     << res.getAmountPaid() << "\n"
                     << "Status: " << (res.isCancelled() ? "Cancelled" : "Active") << "\n"
                     << "-----------------\n";
        }
    }
    
    if (!found) {
        std::cout << "No reservations found.\n";
    }
}

void AirlineSystem::listPassengerReservations(int passenger_id) {
    auto passenger = findPassenger(passenger_id);
    if (!passenger) {
        throw PassengerNotFoundException();
    }

    std::cout << "\nReservations for " << passenger->getName() << ":\n"
              << "-----------------\n";
    bool found = false;
    
    for (const auto& res : reservations) {
        if (res.isDeleted() || res.getPassengerId() != passenger_id) continue;
        
        auto flight = findFlight(res.getFlightId());
        if (flight) {
            found = true;
            std::cout << "Reservation ID: " << res.getReservationId() << "\n"
                     << "Flight: " << flight->getFlightNumber() 
                     << " (" << flight->getOrigin() << " -> " << flight->getDestination() << ")\n"
                     << "Amount Paid: $" << std::fixed << std::setprecision(2) 
                     << res.getAmountPaid() << "\n"
                     << "Status: " << (res.isCancelled() ? "Cancelled" : "Active") << "\n"
                     << "-----------------\n";
        }
    }
    
    if (!found) {
        std::cout << "No reservations found for this passenger.\n";
    }
}

bool AirlineSystem::updatePassenger(int passenger_id, const std::string& name,
                                  const std::string& passport_number,
                                  const std::string& national_id,
                                  const std::string& nationality) {
    auto passenger = findPassenger(passenger_id);
    if (!passenger) {
        throw PassengerNotFoundException();
    }

    // Check if new passport or national ID is already taken by another passenger
    if (isPassportNumberTaken(passport_number, passenger_id)) {
        throw AirlineException("This passport number is already registered to another passenger");
    }
    if (isNationalIdTaken(national_id, passenger_id)) {
        throw AirlineException("This national ID is already registered to another passenger");
    }

    passenger->setName(name);
    passenger->setPassportNumber(passport_number);
    passenger->setNationalId(national_id);
    passenger->setNationality(nationality);

    markDataAsChanged();
    autoSave();
    return true;
}

bool AirlineSystem::deleteFlight(int flight_id) {
    auto flight = findFlight(flight_id);
    if (!flight) {
        throw FlightNotFoundException();
    }

    // Check if flight has active reservations
    auto hasActiveReservations = std::any_of(reservations.begin(), reservations.end(),
        [flight_id](const Reservation& r) {
            return r.getFlightId() == flight_id && !r.isCancelled() && !r.isDeleted();
        });

    if (hasActiveReservations) {
        throw AirlineException("Cannot delete flight with active reservations");
    }

    flight->softDelete();
    markDataAsChanged();
    autoSave();
    return true;
}

bool AirlineSystem::deletePassenger(int passenger_id) {
    auto passenger = findPassenger(passenger_id);
    if (!passenger) {
        throw PassengerNotFoundException();
    }

    // Check for active reservations
    auto hasActiveReservations = std::any_of(reservations.begin(), reservations.end(),
        [passenger_id](const Reservation& r) {
            return r.getPassengerId() == passenger_id && !r.isCancelled() && !r.isDeleted();
        });

    if (hasActiveReservations) {
        throw AirlineException("Cannot delete passenger with active reservations");
    }

    passenger->softDelete();
    markDataAsChanged();
    autoSave();
    return true;
}

bool AirlineSystem::isFlightCompleted(const Flight& flight) const {
    return flight.getDepartureTime() < std::time(nullptr);
}

bool AirlineSystem::isFlightOnDate(const Flight& flight, time_t date) const {
    time_t flight_time = flight.getDepartureTime();
    tm* flight_tm = localtime(&flight_time);
    tm* date_tm = localtime(&date);
    return flight_tm->tm_year == date_tm->tm_year &&
           flight_tm->tm_mon == date_tm->tm_mon &&
           flight_tm->tm_mday == date_tm->tm_mday;
}

void AirlineSystem::generateReservationsReport(const std::string& filename, bool futureOnly, bool completedOnly, bool refundedOnly) {
    std::ofstream outfile(filename);
    if (!outfile) throw FileOperationException("Could not create report file");

    outfile << "Reservation ID,Passenger Name,Flight Number,Date,Status,Amount\n";

    for (const auto& res : reservations) {
        if (res.isDeleted()) continue;

        auto flight = findFlight(res.getFlightId());
        auto passenger = findPassenger(res.getPassengerId());
        if (!flight || !passenger) continue;

        bool isCompleted = isFlightCompleted(*flight);
        
        if (futureOnly && isCompleted) continue;
        if (completedOnly && !isCompleted) continue;
        if (refundedOnly && !res.isCancelled()) continue;

        std::time_t t = flight->getDepartureTime();
        char date_str[11];
        std::strftime(date_str, sizeof(date_str), "%Y-%m-%d", std::localtime(&t));

        outfile << res.getReservationId() << ","
             << passenger->getName() << ","
             << flight->getFlightNumber() << ","
             << date_str << ","
             << (res.isCancelled() ? "Refunded" : (isCompleted ? "Completed" : "Future")) << ","
             << res.getAmountPaid() << "\n";
    }
}

void AirlineSystem::generateFlightPassengersReport(const std::string& filename, int flight_id) {
    auto flight = findFlight(flight_id);
    if (!flight) throw FlightNotFoundException();

    std::ofstream outfile(filename);
    if (!outfile) throw FileOperationException("Could not create report file");

    outfile << "Flight: " << flight->getFlightNumber() << "\n";
    outfile << "Passenger ID,Name,Passport,Nationality,Status\n";

    for (const auto& res : reservations) {
        if (res.isDeleted() || res.getFlightId() != flight_id) continue;

        auto passenger = findPassenger(res.getPassengerId());
        if (!passenger) continue;

        outfile << passenger->getPassengerId() << ","
             << passenger->getName() << ","
             << passenger->getPassportNumber() << ","
             << passenger->getNationality() << ","
             << (res.isCancelled() ? "Cancelled" : "Confirmed") << "\n";
    }
}

void AirlineSystem::generateFlightsByDateReport(const std::string& filename, time_t date) {
    std::ofstream outfile(filename);
    if (!outfile) throw FileOperationException("Could not create report file");

    outfile << "Flight Number,Origin,Destination,Time,Available Seats,Status\n";

    for (const auto& flight : flights) {
        if (flight.isDeleted() || !isFlightOnDate(flight, date)) continue;

        std::time_t t = flight.getDepartureTime();
        char time_str[9];
        std::strftime(time_str, sizeof(time_str), "%H:%M:%S", std::localtime(&t));

        outfile << flight.getFlightNumber() << ","
             << flight.getOrigin() << ","
             << flight.getDestination() << ","
             << time_str << ","
             << flight.getAvailableSeats() << ","
             << (isFlightCompleted(flight) ? "Completed" : "Scheduled") << "\n";
    }
}

void AirlineSystem::generateFutureFlightsReport(const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile) throw FileOperationException("Could not create report file");

    outfile << "Flight Number,Origin,Destination,Date,Time,Available Seats,Price\n";

    time_t now = std::time(nullptr);
    for (const auto& flight : flights) {
        if (flight.isDeleted() || flight.getDepartureTime() <= now) continue;

        std::time_t t = flight.getDepartureTime();
        char date_str[11], time_str[9];
        std::strftime(date_str, sizeof(date_str), "%Y-%m-%d", std::localtime(&t));
        std::strftime(time_str, sizeof(time_str), "%H:%M:%S", std::localtime(&t));

        outfile << flight.getFlightNumber() << ","
             << flight.getOrigin() << ","
             << flight.getDestination() << ","
             << date_str << ","
             << time_str << ","
             << flight.getAvailableSeats() << ","
             << flight.getTicketPrice() << "\n";
    }
}

void AirlineSystem::generatePassengerTripsReport(const std::string& filename, int passenger_id, bool futureOnly, bool refundedOnly) {
    auto passenger = findPassenger(passenger_id);
    if (!passenger) throw PassengerNotFoundException();

    std::ofstream outfile(filename);
    if (!outfile) throw FileOperationException("Could not create report file");

    outfile << "Passenger: " << passenger->getName() << "\n";
    outfile << "Flight Number,Origin,Destination,Date,Status,Amount\n";

    for (const auto& res : reservations) {
        if (res.isDeleted() || res.getPassengerId() != passenger_id) continue;
        if (refundedOnly && !res.isCancelled()) continue;

        auto flight = findFlight(res.getFlightId());
        if (!flight) continue;

        bool isCompleted = isFlightCompleted(*flight);
        if (futureOnly && isCompleted) continue;

        std::time_t t = flight->getDepartureTime();
        char date_str[11];
        std::strftime(date_str, sizeof(date_str), "%Y-%m-%d", std::localtime(&t));

        outfile << flight->getFlightNumber() << ","
             << flight->getOrigin() << ","
             << flight->getDestination() << ","
             << date_str << ","
             << (res.isCancelled() ? "Refunded" : (isCompleted ? "Completed" : "Future")) << ","
             << res.getAmountPaid() << "\n";
    }
}
