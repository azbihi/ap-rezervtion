#pragma once
#include <string>
#include <vector>
#include "Passenger.h"
#include "Flight.h"
#include "Reservation.h"
#include "AirlineExceptions.h"

class FileManager {
private:
    const std::string PASSENGERS_FILE = "passengers.csv";
    const std::string FLIGHTS_FILE = "flights.csv";
    const std::string RESERVATIONS_FILE = "reservations.csv";

    void ensureDirectoryExists();

public:
    // Load operations
    std::vector<Passenger> loadPassengers();
    std::vector<Flight> loadFlights();
    std::vector<Reservation> loadReservations();

    // Save operations
    void savePassengers(const std::vector<Passenger>& passengers);
    void saveFlights(const std::vector<Flight>& flights);
    void saveReservations(const std::vector<Reservation>& reservations);

    // Report generation
    void generateReport(const std::string& filename, const std::string& content);
    void generatePassengerReport(const std::string& filename, 
                               const Passenger& passenger,
                               const std::vector<Reservation>& reservations);
    void generateFlightReport(const std::string& filename, 
                            const Flight& flight,
                            const std::vector<Reservation>& reservations,
                            const std::vector<Passenger>& passengers);
    void generateReservationsReport(const std::string& filename,
                                  const std::vector<Reservation>& reservations,
                                  const std::vector<Passenger>& passengers,
                                  const std::vector<Flight>& flights);

    // Validation methods
    void validatePassengerData(const Passenger& passenger);
    void validateFlightData(const Flight& flight);
    void validateReservationData(const Reservation& reservation);
};
