#pragma once
#include <stdexcept>
#include <string>

class AirlineException : public std::runtime_error {
public:
    explicit AirlineException(const std::string& message) 
        : std::runtime_error(message) {}
};

class FlightCompletedException : public AirlineException {
public:
    FlightCompletedException() : AirlineException("Flight has already departed") {}
};

class RefundNotAllowedException : public AirlineException {
public:
    RefundNotAllowedException() : AirlineException("Refund not allowed within 24 hours of departure") {}
};

class InsufficientBalanceException : public AirlineException {
public:
    InsufficientBalanceException() : AirlineException("Insufficient balance in wallet") {}
};

class PassengerNotFoundException : public AirlineException {
public:
    PassengerNotFoundException() : AirlineException("Passenger not found") {}
};

class FlightNotFoundException : public AirlineException {
public:
    FlightNotFoundException() : AirlineException("Flight not found") {}
};

class ReservationNotFoundException : public AirlineException {
public:
    ReservationNotFoundException() : AirlineException("Reservation not found") {}
};

class InvalidInputException : public AirlineException {
public:
    InvalidInputException(const std::string& field) 
        : AirlineException("Invalid input for field: " + field) {}
};

class FileOperationException : public AirlineException {
public:
    FileOperationException(const std::string& message) 
        : AirlineException("File operation failed: " + message) {}
};
