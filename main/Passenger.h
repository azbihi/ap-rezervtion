#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>

class Passenger {
private:
    int passenger_id;
    std::string full_name;
    std::string passport_number;
    std::string national_code;
    std::string nationality;
    double wallet_balance;
    bool is_deleted;

public:
    Passenger();
    Passenger(int id, const std::string& name, const std::string& passport, const std::string& nationalCode, const std::string& nationality, double balance);

    int getPassengerId() const;
    std::string getFullName() const;
    std::string getPassportNumber() const;
    std::string getNationalCode() const;
    std::string getNationality() const;
    double getWalletBalance() const;
    bool isDeleted() const;

    void setFullName(const std::string& name);
    void setPassportNumber(const std::string& passport);
    void setNationalCode(const std::string& code);
    void setNationality(const std::string& nationality);
    void setWalletBalance(double balance);
    void markDeleted();

    static Passenger fromCSV(const std::string& line);
    std::string toCSV() const;
};

#endif
