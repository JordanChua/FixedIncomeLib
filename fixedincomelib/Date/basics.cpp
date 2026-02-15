#include "basics.h"
#include <regex>
#include <string>
#include <string_view>

namespace fixedincomelib {
    // Regex check to make sure input string format is '%d-%m-%Y' 
    // Also does an extra check depending on the month to ensure the day doesnt exceed the number of days of the month
    inline void validate_dd_mm_yyyy(std::string_view s) {
        if (!is_dd_mm_yyyy(s)) {
            throw std::invalid_argument("Expected date format dd-mm-YYYY");
        }
    
        int d = std::stoi(std::string(s.substr(0,2)));
        int m = std::stoi(std::string(s.substr(3,2)));
        int y = std::stoi(std::string(s.substr(6,4)));
    
        // Basic month/day bounds already enforced by regex, now check real calendar.
        auto is_leap = [](int yy){
            return (yy % 4 == 0 && yy % 100 != 0) || (yy % 400 == 0);
        };
        int mdays[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
        if (m == 2 && is_leap(y)) mdays[2] = 29;
    
        if (d > mdays[m]) {
            throw std::invalid_argument("Invalid calendar date (day exceeds month length)");
        }
    }
    QuantLib::Date date_from_iso(std::string_view iso) {
        // In c++ of Quantlib, we need to give the day, month then year to the Date constructor 
        // We only allow the DD-MM-YYYY format for now 
        validate_dd_mm_yyyy(iso); 

        int d = std::stoi(std::string(s.substr(0,2)));
        int m = std::stoi(std::string(s.substr(3,2)));
        int y = std::stoi(std::string(s.substr(6,4)));
        
        return QuantLib::Date(d, QuantLib::Month(m), y); // Month is a strongly typed enum hence the need to do this
    }

    // The first const makes sure the returned object cannot be modified through these accessors 
    // The second const is referring to a constant func, i.e *this cannot be modified
    const Date& TermOrTerminationDate::get_date() const {
        if (!is_date()) 
            throw std::logic_error("Not a Date!")
        return std::get<Date>(val_); //syntax for getting the type from a variant
    }

    const Period& TermOrTerminationDate::get_ter    m() const {
        if (!is_term()) 
            throw std::logic_error("Not a Term!")
        return std::get<Period>(val_); //syntax for getting the type from a variant
    }
}