#pragma once
#include <stdexcept>
#include <string>

class AirlineException : public std::runtime_error {
public:
    explicit AirlineException(const std::string& message) 
        : std::runtime_error(message) {}
};

class PassengerNotFoundException : public AirlineException {
public:
    PassengerNotFoundException() : AirlineException("Passenger not found") {}
};

class FlightNotFoundException : public AirlineException {
public:
    FlightNotFoundException() : AirlineException("Flight not found") {}
};

class InsufficientBalanceException : public AirlineException {
public:
    InsufficientBalanceException() : AirlineException("Insufficient balance") {}
};

class InvalidInputException : public AirlineException {
public:
    explicit InvalidInputException(const std::string& field) 
        : AirlineException("Invalid input for: " + field) {}
};

class FileOperationException : public AirlineException {
public:
    explicit FileOperationException(const std::string& operation) 
        : AirlineException("File operation failed: " + operation) {}
};

class ReservationNotFoundException : public AirlineException {
public:
    ReservationNotFoundException() : AirlineException("Reservation not found") {}
};

class FlightCompletedException : public AirlineException {
public:
    FlightCompletedException() : AirlineException("Flight is already completed") {}
};

class RefundNotAllowedException : public AirlineException {
public:
    RefundNotAllowedException() : AirlineException("Refund is not allowed due to time constraints") {}
};
