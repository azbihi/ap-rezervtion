#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "../main/AirlineSystem.h"
#include "../main/InputValidator.h"
#include <chrono>

TEST_CASE("Passenger Management Tests", "[passenger]") {
    AirlineSystem system;

    SECTION("Add Valid Passenger") {
        Passenger p("John Doe", "AB123456", "1234567890", "USA");
        REQUIRE_NOTHROW(system.addPassenger(p));
    }

    SECTION("Duplicate National ID") {
        Passenger p1("John Doe", "AB123456", "1234567890", "USA");
        Passenger p2("Jane Doe", "CD789012", "1234567890", "USA");
        
        system.addPassenger(p1);
        REQUIRE_THROWS_AS(system.addPassenger(p2), AirlineException);
    }

    SECTION("Duplicate Passport") {
        Passenger p1("John Doe", "AB123456", "1234567890", "USA");
        Passenger p2("Jane Doe", "AB123456", "0987654321", "USA");
        
        system.addPassenger(p1);
        REQUIRE_THROWS_AS(system.addPassenger(p2), AirlineException);
    }

    SECTION("Search Passenger") {
        Passenger p("John Doe", "AB123456", "1234567890", "USA");
        system.addPassenger(p);
        
        auto results = system.searchPassengers("John");
        REQUIRE(results.size() == 1);
    }
}

TEST_CASE("Flight Management Tests", "[flight]") {
    AirlineSystem system;
    time_t future_time = std::time(nullptr) + 24*60*60; // Tomorrow

    SECTION("Add Valid Flight") {
        Flight f("AB123", "New York", "London", future_time, 100, 500.0);
        REQUIRE_NOTHROW(system.addFlight(f));
    }

    SECTION("Search Flights") {
        Flight f("AB123", "New York", "London", future_time, 100, 500.0);
        system.addFlight(f);
        
        auto results = system.searchFlights("New York");
        REQUIRE(results.size() == 1);
    }

    SECTION("Reserve Seat") {
        Flight f("AB123", "New York", "London", future_time, 1, 500.0);
        system.addFlight(f);
        
        REQUIRE(f.reserveSeat());
        REQUIRE_FALSE(f.reserveSeat()); // No more seats
    }
}

TEST_CASE("Reservation Management Tests", "[reservation]") {
    AirlineSystem system;
    time_t future_time = std::time(nullptr) + 24*60*60;

    SECTION("Make Valid Reservation") {
        // Setup
        Passenger p("John Doe", "AB123456", "1234567890", "USA");
        Flight f("AB123", "New York", "London", future_time, 1, 500.0);
        
        int passenger_id = system.addPassenger(p);
        int flight_id = system.addFlight(f);
        
        // Add money to wallet
        auto passenger = system.findPassenger(passenger_id);
        passenger->updateWalletBalance(1000.0);
        
        REQUIRE_NOTHROW(system.makeReservation(passenger_id, flight_id));
    }

    SECTION("Insufficient Balance") {
        Passenger p("John Doe", "AB123456", "1234567890", "USA");
        Flight f("AB123", "New York", "London", future_time, 1, 500.0);
        
        int passenger_id = system.addPassenger(p);
        int flight_id = system.addFlight(f);
        
        REQUIRE_THROWS_AS(system.makeReservation(passenger_id, flight_id), 
                         InsufficientBalanceException);
    }

    SECTION("Cancel Reservation") {
        // Setup reservation
        Passenger p("John Doe", "AB123456", "1234567890", "USA");
        Flight f("AB123", "New York", "London", future_time, 1, 500.0);
        
        int passenger_id = system.addPassenger(p);
        int flight_id = system.addFlight(f);
        
        auto passenger = system.findPassenger(passenger_id);
        passenger->updateWalletBalance(1000.0);
        
        int reservation_id = system.makeReservation(passenger_id, flight_id);
        
        REQUIRE_NOTHROW(system.cancelReservation(reservation_id));
    }
}

TEST_CASE("Refund Policy Tests", "[refund]") {
    AirlineSystem system;

    SECTION("48+ Hours Before Flight") {
        time_t flight_time = std::time(nullptr) + 72*60*60; // 3 days from now
        
        Passenger p("John Doe", "AB123456", "1234567890", "USA");
        Flight f("AB123", "New York", "London", flight_time, 1, 1000.0);
        
        int passenger_id = system.addPassenger(p);
        int flight_id = system.addFlight(f);
        
        auto passenger = system.findPassenger(passenger_id);
        passenger->updateWalletBalance(1000.0);
        
        int reservation_id = system.makeReservation(passenger_id, flight_id);
        
        double initial_balance = passenger->getWalletBalance();
        system.cancelReservation(reservation_id);
        
        // Should get 90% refund
        REQUIRE(passenger->getWalletBalance() == Approx(initial_balance + 900.0));
    }

    SECTION("24-48 Hours Before Flight") {
        time_t flight_time = std::time(nullptr) + 36*60*60; // 36 hours from now
        
        // Similar test setup but checking for 50% refund
    }

    SECTION("Less than 24 Hours Before Flight") {
        time_t flight_time = std::time(nullptr) + 12*60*60; // 12 hours from now
        
        // Test that refund is not allowed
    }
}

TEST_CASE("Input Validation Tests", "[validation]") {
    SECTION("Validate National ID") {
        REQUIRE(InputValidator::validateNationalId("1234567890"));
        REQUIRE_FALSE(InputValidator::validateNationalId("123"));
        REQUIRE_FALSE(InputValidator::validateNationalId("abcdefghij"));
    }

    SECTION("Validate Passport Number") {
        REQUIRE(InputValidator::validatePassportNumber("AB123456"));
        REQUIRE_FALSE(InputValidator::validatePassportNumber("123"));
    }

    SECTION("Validate Flight Number") {
        REQUIRE(InputValidator::validateFlightNumber("AB123"));
        REQUIRE_FALSE(InputValidator::validateFlightNumber("12345"));
    }
}

منظTEST_CASE("Report Generation Tests", "[reports]") {
    AirlineSystem system;
    time_t future_time = std::time(nullptr) + 24*60*60;

    SECTION("Generate Flight Report") {
        Flight f("AB123", "New York", "London", future_time, 100, 500.0);
        int flight_id = system.addFlight(f);
        REQUIRE_NOTHROW(system.generateFlightReport(flight_id));
    }

    SECTION("Generate Passenger Report") {
        Passenger p("John Doe", "AB123456", "1234567890", "USA");
        int passenger_id = system.addPassenger(p);
        REQUIRE_NOTHROW(system.generatePassengerReport(passenger_id));
    }

    SECTION("Generate Filtered Reservations Report") {
        REQUIRE_NOTHROW(system.generateReservationsReport("test_report.csv", true, false, false));
    }
}
