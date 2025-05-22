#pragma once
#include <string>
#include <ctime>

class InputValidator {
public:
    static bool isNumeric(const std::string& str);
    static bool validateNationalId(const std::string& id); // exactly 10 digits
    static bool validatePassportNumber(const std::string& passport); // 8-9 digits/chars
    static bool validateFlightNumber(const std::string& flight); // 5-6 chars
    static bool validateDate(int year, int month, int day);
    static bool validateTime(int hour, int minute);

    static int getValidatedInteger(const std::string& prompt, int minDigits = 1, int maxDigits = 10);
    static double getValidatedDouble(const std::string& prompt);
    static std::string getValidatedFlightNumber(const std::string& prompt);
    static time_t getValidatedDateTime(const std::string& prompt);
};
