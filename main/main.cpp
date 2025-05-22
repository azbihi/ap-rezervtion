#include <iostream>
#include <algorithm>  // Add this for std::all_of
#include "AirlineSystem.h"
#include "InputValidator.h"
#include <limits>
#include <iomanip>

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMainMenu() {
    std::cout << "\n=== Airline Reservation System ===\n"
              << "1. Passenger Management\n"
              << "2. Flight Management\n"
              << "3. Reservation Management\n"
              << "4. Reports\n"
              << "0. Exit\n"
              << "Choose an option: ";
}

int getValidMenuChoice() {
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        
        try {
            if (input.empty()) {
                std::cout << "Please enter a number: ";
                continue;
            }
            
            // Check if input contains only digits
            if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
                std::cout << "Please enter numbers only: ";
                continue;
            }
            
            return std::stoi(input);
        } catch (const std::exception&) {
            std::cout << "Invalid input. Please enter a valid number: ";
        }
    }
}

void passengerMenu(AirlineSystem& system) {
    while (true) {
        clearScreen();
        std::cout << "\n=== Passenger Management ===\n"
                  << "1. Add New Passenger\n"
                  << "2. Search Passengers\n"
                  << "3. Update Wallet Balance\n"
                  << "4. List All Passengers\n"
                  << "5. Edit Passenger\n"
                  << "6. Delete Passenger\n"
                  << "7. Back to Main Menu\n"
                  << "Choose an option: ";

        int choice = getValidMenuChoice();

        switch (choice) {
            case 1: {
                std::string name, passport, national_id, nationality;
                std::cout << "Enter name: ";
                std::getline(std::cin, name);
                
                do {
                    std::cout << "Enter passport number: ";
                    std::getline(std::cin, passport);
                    if (!InputValidator::validatePassportNumber(passport)) {
                        std::cout << "Invalid passport number format (must be 8-9 characters)\n";
                        continue;
                    }
                    if (system.isPassportNumberTaken(passport)) {
                        std::cout << "This passport number is already registered\n";
                        continue;
                    }
                    break;
                } while (true);
                
                do {
                    std::cout << "Enter national ID: ";
                    std::getline(std::cin, national_id);
                    if (!InputValidator::validateNationalId(national_id)) {
                        std::cout << "Invalid national ID (must be exactly 10 digits)\n";
                        continue;
                    }
                    if (system.isNationalIdTaken(national_id)) {
                        std::cout << "This national ID is already registered\n";
                        continue;
                    }
                    break;
                } while (true);
                
                std::cout << "Enter nationality: ";
                std::getline(std::cin, nationality);

                Passenger p(name, passport, national_id, nationality);
                int id = system.addPassenger(p);
                std::cout << "Passenger added successfully. ID: " << id << std::endl;
                break;
            }
            case 2: {
                std::string search_term;
                std::cout << "Enter search term: ";
                std::getline(std::cin, search_term);
                
                auto results = system.searchPassengers(search_term);
                for (const auto& p : results) {
                    std::cout << "ID: " << p.getPassengerId() 
                              << ", Name: " << p.getName()
                              << ", Balance: " << p.getWalletBalance() << std::endl;
                }
                break;
            }
            case 3: {
                int id = InputValidator::getValidatedInteger("Enter passenger ID: ", 1, 6);
                double amount = InputValidator::getValidatedDouble("Enter amount to add: ");
                
                auto passenger = system.findPassenger(id);
                if (passenger) {
                    passenger->updateWalletBalance(amount);
                    std::cout << "Balance updated successfully" << std::endl;
                } else {
                    std::cout << "Passenger not found" << std::endl;
                }
                break;
            }
            case 4: {
                system.listAllPassengers();
                break;
            }
            case 5: {
                int id = InputValidator::getValidatedInteger("Enter passenger ID to edit: ", 1, 6);
                auto passenger = system.findPassenger(id);
                if (!passenger) {
                    std::cout << "Passenger not found" << std::endl;
                    break;
                }

                system.displayPassengerDetails(*passenger);
                std::cout << "\nEnter new details (or press Enter to keep current value):\n";

                std::string name, passport, national_id, nationality;
                std::string input;

                std::cout << "Enter new name (current: " << passenger->getName() << "): ";
                std::getline(std::cin, input);
                name = input.empty() ? passenger->getName() : input;

                bool valid_passport = false;
                do {
                    std::cout << "Enter new passport number (current: " << passenger->getPassportNumber() << "): ";
                    std::getline(std::cin, input);
                    passport = input.empty() ? passenger->getPassportNumber() : input;
                    
                    if (!InputValidator::validatePassportNumber(passport)) {
                        std::cout << "Invalid passport number format (must be 8-9 characters)\n";
                        continue;
                    }
                    if (system.isPassportNumberTaken(passport, id)) {
                        std::cout << "This passport number is already registered to another passenger\n";
                        continue;
                    }
                    valid_passport = true;
                } while (!valid_passport && !input.empty());

                bool valid_national_id = false;
                do {
                    std::cout << "Enter new national ID (current: " << passenger->getNationalId() << "): ";
                    std::getline(std::cin, input);
                    national_id = input.empty() ? passenger->getNationalId() : input;
                    
                    if (!InputValidator::validateNationalId(national_id)) {
                        std::cout << "Invalid national ID (must be exactly 10 digits)\n";
                        continue;
                    }
                    if (system.isNationalIdTaken(national_id, id)) {
                        std::cout << "This national ID is already registered to another passenger\n";
                        continue;
                    }
                    valid_national_id = true;
                } while (!valid_national_id && !input.empty());

                std::cout << "Enter new nationality (current: " << passenger->getNationality() << "): ";
                std::getline(std::cin, input);
                nationality = input.empty() ? passenger->getNationality() : input;

                try {
                    if (system.updatePassenger(id, name, passport, national_id, nationality)) {
                        std::cout << "Passenger updated successfully\n";
                    }
                } catch (const std::exception& e) {
                    std::cout << "Error updating passenger: " << e.what() << std::endl;
                }
                break;
            }
            case 6: {
                int id = InputValidator::getValidatedInteger("Enter passenger ID to delete: ", 1, 6);
                try {
                    if (system.deletePassenger(id)) {
                        std::cout << "Passenger deleted successfully\n";
                    }
                } catch (const std::exception& e) {
                    std::cout << "Error deleting passenger: " << e.what() << std::endl;
                }
                break;
            }
            case 7:
                return;
        }
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
}

void flightMenu(AirlineSystem& system) {
    while (true) {
        clearScreen();
        std::cout << "\n=== Flight Management ===\n"
                  << "1. Add New Flight\n"
                  << "2. Search Flights\n"
                  << "3. List All Flights\n"
                  << "4. Delete Flight\n"
                  << "5. Back to Main Menu\n"
                  << "Choose an option: ";

        int choice = getValidMenuChoice();

        switch (choice) {
            case 1: {
                std::string flight_number = InputValidator::getValidatedFlightNumber("Enter flight number (e.g., AB123): ");
                std::string origin, destination;
                int seats;
                double price;
                
                std::cout << "Enter origin: ";
                std::getline(std::cin, origin);
                std::cout << "Enter destination: ";
                std::getline(std::cin, destination);
                
                seats = InputValidator::getValidatedInteger("Enter available seats: ", 1, 3);
                price = InputValidator::getValidatedDouble("Enter ticket price: ");
                
                time_t departure_time = InputValidator::getValidatedDateTime("Enter departure date and time:");
                
                Flight f(flight_number, origin, destination, departure_time, seats, price);
                system.addFlight(f);
                std::cout << "Flight added successfully\n";
                break;
            }
            case 2: {
                std::string search_term;
                std::cout << "Enter search term (flight number/origin/destination): ";
                std::getline(std::cin, search_term);
                
                auto results = system.searchFlights(search_term);
                if (results.empty()) {
                    std::cout << "No flights found.\n";
                } else {
                    std::cout << "\nSearch Results:\n";
                    for (const auto& f : results) {
                        system.displayFlightDetails(f);
                        std::cout << "--------------\n";
                    }
                }
                break;
            }
            case 3: {
                system.listAllFlights();
                break;
            }
            case 4: {
                int flight_id = InputValidator::getValidatedInteger("Enter flight ID to delete: ", 1, 6);
                try {
                    if (system.deleteFlight(flight_id)) {
                        std::cout << "Flight deleted successfully\n";
                    }
                } catch (const std::exception& e) {
                    std::cout << "Error deleting flight: " << e.what() << std::endl;
                }
                break;
            }
            case 5:
                return;
            default:
                std::cout << "Invalid option!\n";
        }
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
}

void reservationMenu(AirlineSystem& system) {
    while (true) {
        clearScreen();
        std::cout << "\n=== Reservation Management ===\n"
                  << "1. Make New Reservation\n"
                  << "2. Cancel Reservation\n"
                  << "3. List All Reservations\n"
                  << "4. List Passenger Reservations\n"
                  << "5. Back to Main Menu\n"
                  << "Choose an option: ";

        int choice = getValidMenuChoice();

        switch (choice) {
            case 1: {
                int passenger_id = InputValidator::getValidatedInteger("Enter passenger ID: ", 1, 6);
                int flight_id = InputValidator::getValidatedInteger("Enter flight ID: ", 1, 6);

                try {
                    int reservation_id = system.makeReservation(passenger_id, flight_id);
                    std::cout << "Reservation successful! ID: " << reservation_id << std::endl;
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 2: {
                int reservation_id = InputValidator::getValidatedInteger("Enter reservation ID to cancel: ", 1, 6);

                try {
                    if (system.cancelReservation(reservation_id)) {
                        std::cout << "Reservation cancelled successfully\n";
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 3: {
                system.listAllReservations();
                break;
            }
            case 4: {
                int passenger_id = InputValidator::getValidatedInteger("Enter passenger ID: ", 1, 6);
                try {
                    system.listPassengerReservations(passenger_id);
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 5:
                return;
        }
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
}

void reportMenu(AirlineSystem& system) {
    while (true) {
        clearScreen();
        std::cout << "\n=== Reports ===\n"
                  << "1. Basic Flight Report\n"
                  << "2. Basic Passenger Report\n"
                  << "3. Basic Reservation Report\n"
                  << "4. Filtered Reservations Report\n"
                  << "5. Flight Passengers Report\n"
                  << "6. Daily Flights Report\n"
                  << "7. Future Flights Report\n"
                  << "8. Passenger Trips Report\n"
                  << "9. Back to Main Menu\n"
                  << "Choose an option: ";

        int choice = getValidMenuChoice();

        try {
            switch (choice) {
                case 1: {
                    int flight_id = InputValidator::getValidatedInteger("Enter flight ID: ", 1, 6);
                    system.generateFlightReport(flight_id);
                    std::cout << "Report generated successfully in flight_report_" 
                             << flight_id << ".txt\n";
                    break;
                }
                case 2: {
                    int passenger_id = InputValidator::getValidatedInteger("Enter passenger ID: ", 1, 6);
                    system.generatePassengerReport(passenger_id);
                    std::cout << "Report generated successfully in passenger_report_" 
                             << passenger_id << ".txt\n";
                    break;
                }
                case 3: {
                    system.generateReservationReport();
                    std::cout << "Report generated successfully in reservations_report.txt\n";
                    break;
                }
                case 4: {
                    std::cout << "Select filter type:\n"
                              << "1. Future reservations only\n"
                              << "2. Completed reservations only\n"
                              << "3. Refunded reservations only\n"
                              << "Choose option: ";
                    
                    int filter = getValidMenuChoice();
                    std::string filename = "filtered_reservations.csv";
                    
                    switch(filter) {
                        case 1:
                            system.generateReservationsReport(filename, true, false, false);
                            break;
                        case 2:
                            system.generateReservationsReport(filename, false, true, false);
                            break;
                        case 3:
                            system.generateReservationsReport(filename, false, false, true);
                            break;
                        default:
                            std::cout << "Invalid filter option\n";
                            break;
                    }
                    std::cout << "Report generated successfully in " << filename << "\n";
                    break;
                }
                case 5: {
                    int flight_id = InputValidator::getValidatedInteger("Enter flight ID: ", 1, 6);
                    std::string filename = "flight_" + std::to_string(flight_id) + "_passengers.csv";
                    system.generateFlightPassengersReport(filename, flight_id);
                    std::cout << "Report generated successfully in " << filename << "\n";
                    break;
                }
                case 6: {
                    time_t date = InputValidator::getValidatedDateTime("Enter date for flights report:");
                    std::string filename = "daily_flights.csv";
                    system.generateFlightsByDateReport(filename, date);
                    std::cout << "Report generated successfully in " << filename << "\n";
                    break;
                }
                case 7: {
                    std::string filename = "future_flights.csv";
                    system.generateFutureFlightsReport(filename);
                    std::cout << "Report generated successfully in " << filename << "\n";
                    break;
                }
                case 8: {
                    int passenger_id = InputValidator::getValidatedInteger("Enter passenger ID: ", 1, 6);
                    std::cout << "Select filter type:\n"
                              << "1. Future trips only\n"
                              << "2. Refunded trips only\n"
                              << "3. All trips\n"
                              << "Choose option: ";
                    
                    int filter = getValidMenuChoice();
                    std::string filename = "passenger_" + std::to_string(passenger_id) + "_trips.csv";
                    
                    switch(filter) {
                        case 1:
                            system.generatePassengerTripsReport(filename, passenger_id, true, false);
                            break;
                        case 2:
                            system.generatePassengerTripsReport(filename, passenger_id, false, true);
                            break;
                        case 3:
                            system.generatePassengerTripsReport(filename, passenger_id, false, false);
                            break;
                        default:
                            std::cout << "Invalid filter option\n";
                            break;
                    }
                    std::cout << "Report generated successfully in " << filename << "\n";
                    break;
                }
                case 9:
                    return;
                default:
                    std::cout << "Invalid option!\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error generating report: " << e.what() << std::endl;
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
}

int main() {
    try {
        AirlineSystem system;
        
        while (true) {
            clearScreen();
            displayMainMenu();
            
            int choice = getValidMenuChoice();
            
            switch (choice) {
                case 1:
                    passengerMenu(system);
                    break;
                case 2:
                    flightMenu(system);
                    break;
                case 3:
                    reservationMenu(system);
                    break;
                case 4:
                    reportMenu(system);
                    break;
                case 0:
                    std::cout << "Thank you for using our system!\n";
                    return 0;
                default:
                    std::cout << "Invalid option!\n";
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
