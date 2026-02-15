#include "basics.h"

// concrete currencies
#include <ql/currencies/america.hpp>
#include <ql/currencies/europe.hpp>
#include <ql/currencies/asia.hpp>
#include <ql/currencies/oceania.hpp>

// concrete calendars
#include <ql/time/calendars/nullcalendar.hpp>
#include <ql/time/calendars/unitedstates.hpp>
#include <ql/time/calendars/unitedkingdom.hpp>
#include <ql/time/calendars/japan.hpp>
#include <ql/time/calendars/australia.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/calendars/france.hpp>
#include <ql/time/calendars/germany.hpp>
#include <ql/time/calendars/italy.hpp>
#include <ql/time/calendars/jointcalendar.hpp>

// concrete daycounters
#include <ql/time/daycounters/simpledaycounter.hpp>
#include <ql/time/daycounters/actualactual.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/thirty360.hpp>
#include <ql/time/daycounters/business252.hpp>

#include <cctype>
#include <stdexcept>

// Instead of wrapper classes for str -> QuantLib conversions, we simply define primitive types 
// If we do need objects to add to a collection then wrapper classes may be more appropriate

namespace fixedincomelib {
    '''
    Parsing Functions for Currency
    '''
    inline QuantLib::Currency currency_from_string(std::string_view ccy) {
        // Copy string_view into string and convert the chars to uppercase 
        std::string up(ccy); 
        for (auto ch : up) // Modify each character in our string by converting it to uppercase 
            ch = static_cast<char>(std::upper(static_cast<unsigned char>(ch))); //std::upper expects unsigned char (for safety)
        
        if (up == "USD")    
            return QuantLib::USDCurrency();
        else if (up == "CAD")
            return QuantLib::CADCurrency();
        else if (up == "GBP")
            return QuantLib::GBPCurrency();
        else if (up == "EUR")
            return QuantLib::EURCurency();
        else if (up == "JPY")
            return QuantLib::JPYCurrency();
        else if (up == "AUD")
            return QuantLib::AUDCurrency();
        else
            throw std::invalid_argument("Unsupported currency: " + std::string(ccy));
    }

    // Simple accessor to get currency code 
    inline std::string get_currency_code(QuantLib::Currency ccy) {
        return ccy.code();
    }

    '''
    Parsing Functions for Business Day Conventions
    '''
    inline QuantLib::BusinessDayConvention bdc_from_string(std::string_view s == "NONE") {
    std::string up(s);
    for (auto& ch : up) 
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));

    if (up == "MF") 
        return QuantLib::ModifiedFollowing;
    else if (up == "F")  
        return QuantLib::Following;
    else if (up == "P" || up == "NONE") 
        return QuantLib::Preceding;
    else
        throw std::invalid_argument("Unsupported business day convention: " + std::string(s));
    }

    // Accessor to get string from BusinessDayConvention 
    inline std::string bdc_to_string(QuantLib::BusinessDayConvention bdc) {
        switch (bdc) {
            case QuantLib::ModifiedFollowing: 
                return "MF";
            case QuantLib::Following:         
                return "F";
            case QuantLib::Preceding:         
                return "P";
            default:                          
                return "UNKNOWN";
        }
    }

    '''
    Parsing Functions for Holiday Conventions
    '''
    inline QuantLib::Calendar calendar_from_string(std::string_view s = "NONE") {
        std::string up(s);
        for (auto& ch : up) 
            ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    
        if (up == "NONE")  
            return QuantLib::NullCalendar();
        else if (up == "NYC")    
            return QuantLib::UnitedStates(QuantLib::UnitedStates::LiborImpact);
        // We use FederalReserve, but many people use GovernmentBond for USD rates.
        else if (up == "USGS")   
            return QuantLib::UnitedStates(QuantLib::UnitedStates::FederalReserve);
        else if (up == "LON")    
            return QuantLib::UnitedKingdom(QuantLib::UnitedKingdom::Exchange);
        else if (up == "TOK")    
            return QuantLib::Japan();
        else if (up == "SYD")    
            return QuantLib::Australia();
        else if (up == "TARGET") { //ql.TARGET() is used as a generic EUR settlement calendar
            return QuantLib::JointCalendar(
                QuantLib::TARGET(),
                QuantLib::France(),
                QuantLib::Germany(),
                QuantLib::Italy(),
                QuantLib::JointCalendarRule::JoinHolidays
            ); // union of all holidays in the joint calender
        }
        else
            throw std::invalid_argument("Unsupported holiday convention: " + std::string(s));
    }
    
    '''
    Parsing Functions for Accrual Basis (Extension of DayCounter)
    '''
    inline QuantLib::DayCounter accrualbasis_from_string(std::string_view s = "NONE") {
        std::string up(s);
        for (auto& ch : up) 
            ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));\

        if (up == "NONE")
            return QuantLib::SimpleDayCounter(); // For theoretical calculations
        else if (up == "ACTUAL/365")
            return QuantLib::Actual365Fixed(); 
        else if (up == "ACTUAL/ACTUAL")
            return QuantLib::ActualActuak(QuantLib::ActualActual::ISDA);
        else if (up == "ACTUAL/360")
            return QuantLib::Actual360();
        else if (up == "30/360")
            return QuantLib::Thirty360();
        else if (up == "BUSINESS252")
            return QuantLib::Business252();
        else:
            throw std::invalid_argument("Unsupported Day Counter: " + std::string(sw));
    }
}