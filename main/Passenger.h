#pragma once
#include <string>

class Passenger {
private:
    int passenger_id;
    std::string name;
    std::string passport_number;
    std::string national_id;
    std::string nationality;
    double wallet_balance;
    bool is_deleted;

public:
    Passenger(const std::string& name, const std::string& passport_number,
             const std::string& national_id, const std::string& nationality);
    
    // Getters
    int getPassengerId() const { return passenger_id; }
    std::string getName() const { return name; }
    std::string getPassportNumber() const { return passport_number; }
    std::string getNationalId() const { return national_id; }
    std::string getNationality() const { return nationality; }
    double getWalletBalance() const { return wallet_balance; }
    bool isDeleted() const { return is_deleted; }

    // Setters
    void setName(const std::string& name) { this->name = name; }
    void setPassportNumber(const std::string& passport_number) { this->passport_number = passport_number; }
    void setNationalId(const std::string& national_id) { this->national_id = national_id; }
    void setNationality(const std::string& nationality) { this->nationality = nationality; }
    void updateWalletBalance(double amount) { wallet_balance += amount; }
    void softDelete() { is_deleted = true; }

    // For file operations
    std::string toCSV() const;
    static Passenger fromCSV(const std::string& csv_line);
};
