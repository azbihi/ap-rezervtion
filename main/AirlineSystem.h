#pragma once
#include <vector>
#include <memory>
#include "Passenger.h"
#include "Flight.h"
#include "Reservation.h"
#include "FileManager.h"
#include "AirlineExceptions.h"

class AirlineSystem {
private:
    std::vector<Passenger> passengers;
    std::vector<Flight> flights;
    std::vector<Reservation> reservations;
    FileManager file_manager;
    bool data_changed;

    void validateReservation(int passenger_id, int flight_id);
    void markDataAsChanged() { data_changed = true; }
    void autoSave();

public:
    AirlineSystem();
    ~AirlineSystem();

    // Passenger management
    int addPassenger(const Passenger& passenger);
    Passenger* findPassenger(int passenger_id);
    std::vector<Passenger> searchPassengers(const std::string& search_term);
    bool deletePassenger(int passenger_id);

    // Flight management
    int addFlight(const Flight& flight);
    Flight* findFlight(int flight_id);
    std::vector<Flight> searchFlights(const std::string& search_term);

    // Reservation management
    int makeReservation(int passenger_id, int flight_id);
    bool cancelReservation(int reservation_id);
    Reservation* findReservation(int reservation_id);

    // Report generation
    void generateFlightReport(int flight_id);
    void generatePassengerReport(int passenger_id);
    void generateReservationReport();
    void generateReservationsReport(const std::string& filename, bool futureOnly = false, bool completedOnly = false, bool refundedOnly = false);
    void generateFlightPassengersReport(const std::string& filename, int flight_id);
    void generateFlightsByDateReport(const std::string& filename, time_t date);
    void generateFutureFlightsReport(const std::string& filename);
    void generatePassengerTripsReport(const std::string& filename, int passenger_id, bool futureOnly = false, bool refundedOnly = false);

    // File operations
    void saveAllData();
    void loadAllData();

    // New methods for better error handling and file management
    bool hasUnsavedChanges() const { return data_changed; }
    void forceSync() { saveAllData(); data_changed = false; }
    void ensureFileExists();

    // Add new methods
    bool isNationalIdTaken(const std::string& national_id, int exclude_id = -1);
    bool isPassportNumberTaken(const std::string& passport, int exclude_id = -1);
    void displayPassengerDetails(const Passenger& passenger);
    void listAllPassengers();
    bool updatePassenger(int passenger_id, const std::string& name, 
                        const std::string& passport_number,
                        const std::string& national_id,
                        const std::string& nationality);
    void displayFlightDetails(const Flight& flight);
    void listAllFlights();
    bool deleteFlight(int flight_id);
    void listAllReservations();
    void listPassengerReservations(int passenger_id);

    // Helper methods
    bool isFlightCompleted(const Flight& flight) const;
    bool isFlightOnDate(const Flight& flight, time_t date) const;
};
