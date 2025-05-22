#include "InputValidator.h"
#include <iostream>
#include <algorithm>

bool InputValidator::isNumeric(const std::string& str) {
    return !str.empty() && 
           std::find_if(str.begin(), str.end(), [](char c) { 
               return !std::isdigit(c); 
           }) == str.end();
}

bool InputValidator::validateNationalId(const std::string& id) {
    return isNumeric(id) && id.length() == 10;
}

bool InputValidator::validatePassportNumber(const std::string& passport) {
    return passport.length() >= 8 && passport.length() <= 9;
}

bool InputValidator::validateFlightNumber(const std::string& flight) {
    if (flight.length() < 5 || flight.length() > 6) {
        return false;
    }
    
    // First two characters must be letters
    if (!std::isalpha(flight[0]) || !std::isalpha(flight[1])) {
        return false;
    }
    
    // Remaining characters must be numbers
    for (size_t i = 2; i < flight.length(); i++) {
        if (!std::isdigit(flight[i])) {
            return false;
        }
    }
    
    return true;
}

bool InputValidator::validateDate(int year, int month, int day) {
    if (year < 2024 || year > 2030) return false;
    if (month < 1 || month > 12) return false;
    
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0) days_in_month[2] = 29; // Leap year
    
    return day >= 1 && day <= days_in_month[month];
}

bool InputValidator::validateTime(int hour, int minute) {
    return hour >= 0 && hour < 24 && minute >= 0 && minute < 60;
}

int InputValidator::getValidatedInteger(const std::string& prompt, int minDigits, int maxDigits) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        
        if (!isNumeric(input)) {
            std::cout << "Error: Please enter numbers only.\n";
            continue;
        }
        
        if (input.length() < minDigits || input.length() > maxDigits) {
            std::cout << "Error: Number must be between " << minDigits 
                      << " and " << maxDigits << " digits.\n";
            continue;
        }
        
        try {
            return std::stoi(input);
        } catch (const std::exception&) {
            std::cout << "Error: Invalid number.\n";
        }
    }
}

double InputValidator::getValidatedDouble(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        
        try {
            return std::stod(input);
        } catch (const std::exception&) {
            std::cout << "Error: Please enter a valid number.\n";
        }
    }
}

std::string InputValidator::getValidatedFlightNumber(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        
        if (validateFlightNumber(input)) {
            return input;
        }
        
        std::cout << "Error: Flight number must start with 2 letters followed by 3-4 digits (e.g., AB123)\n";
    }
}

time_t InputValidator::getValidatedDateTime(const std::string& prompt) {
    while (true) {
        std::cout << prompt << "\n";
        int year, month, day, hour, minute;
        
        std::cout << "Year (2024-2030): ";
        year = getValidatedInteger("", 4, 4);
        
        std::cout << "Month (1-12): ";
        month = getValidatedInteger("", 1, 2);
        
        std::cout << "Day: ";
        day = getValidatedInteger("", 1, 2);
        
        std::cout << "Hour (0-23): ";
        hour = getValidatedInteger("", 1, 2);
        
        std::cout << "Minute (0-59): ";
        minute = getValidatedInteger("", 1, 2);

        if (!validateDate(year, month, day)) {
            std::cout << "Invalid date. Please try again.\n";
            continue;
        }

        if (!validateTime(hour, minute)) {
            std::cout << "Invalid time. Please try again.\n";
            continue;
        }

        struct tm timeinfo = {};
        timeinfo.tm_year = year - 1900;
        timeinfo.tm_mon = month - 1;
        timeinfo.tm_mday = day;
        timeinfo.tm_hour = hour;
        timeinfo.tm_min = minute;
        timeinfo.tm_sec = 0;

        time_t result = mktime(&timeinfo);
        if (result == -1) {
            std::cout << "Invalid date/time combination. Please try again.\n";
            continue;
        }

        // Check if date is in the future
        if (result <= std::time(nullptr)) {
            std::cout << "Flight time must be in the future. Please try again.\n";
            continue;
        }

        return result;
    }
}
