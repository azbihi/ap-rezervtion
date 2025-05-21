#include "Passenger.h"
#include <sstream>

Passenger::Passenger() : passenger_id(0), wallet_balance(0.0), is_deleted(false) {}

Passenger::Passenger(int id, const std::string& name, const std::string& passport, const std::string& nationalCode, const std::string& nationality, double balance)
        : passenger_id(id), full_name(name), passport_number(passport), national_code(nationalCode), nationality(nationality), wallet_balance(balance), is_deleted(false) {}

int Passenger::getPassengerId() const { return passenger_id; }
std::string Passenger::getFullName() const { return full_name; }
std::string Passenger::getPassportNumber() const { return passport_number; }
std::string Passenger::getNationalCode() const { return national_code; }
std::string Passenger::getNationality() const { return nationality; }
double Passenger::getWalletBalance() const { return wallet_balance; }
bool Passenger::isDeleted() const { return is_deleted; }

void Passenger::setFullName(const std::string& name) { full_name = name; }
void Passenger::setPassportNumber(const std::string& passport) { passport_number = passport; }
void Passenger::setNationalCode(const std::string& code) { national_code = code; }
void Passenger::setNationality(const std::string& nationality1) { this->nationality = nationality1; }
void Passenger::setWalletBalance(double balance) { wallet_balance = balance; }
void Passenger::markDeleted() { is_deleted = true; }

Passenger Passenger::fromCSV(const std::string& line) {
    std::stringstream ss(line);
    std::string token;
    Passenger p;
    getline(ss, token, ','); p.passenger_id = std::stoi(token);
    getline(ss, p.full_name, ',');
    getline(ss, p.passport_number, ',');
    getline(ss, p.national_code, ',');
    getline(ss, p.nationality, ',');
    getline(ss, token, ','); p.wallet_balance = std::stod(token);
    getline(ss, token, ','); p.is_deleted = (token == "1");
    return p;
}

std::string Passenger::toCSV() const {
    std::stringstream ss;
    ss << passenger_id << "," << full_name << "," << passport_number << "," << national_code << "," << nationality << "," << wallet_balance << "," << (is_deleted ? "1" : "0");
    return ss.str();
}
