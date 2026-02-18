#pragma once

#include <ql/time/date.hpp>
#include <ql/time/period.hpp>
#include <ql/utilities/dataparsers.hpp>

#include <variant>
#include <sstream>   // std::ostringstream
#include <iomanip>   // std::setw, std::setfill
#include <string>
#include <string_view>
#include <stdexcept>
#include <cctype>

namespace fixedincomelib {
    // Extension of Quantlib's date class 
    class Date { 
        public: 
            Date() = default; // Ensure default constructor is initialized (this isn't initialized if we have other constructors)
            
            // Explicit keywords here just to ensure no implicit conversion from accepted constructor inputs to Date object 
            explicit Date(const QuantLib::Date d): d_(d) {} 

            // Here string_view is a non-owning view (pointer + length) of the string that provides read-only interface
            // However we need to ensure underlying outlives string_view object to avoid dangling pointers
            explicit Date(const std::string_view iso): d_(date_from_iso(iso)){} 

            // Functions for the constructor that takes in a string
            void validate_dd_mm_yyyy(std::string_view s);
            QuantLib::Date date_from_iso(std::string_view iso);

            //Accesor
            QuantLib::Date get_date() const {return d_;}
            std::string get_date_str() const {
                std::ostringstream oss;
                oss << std::setw(2) << std::setfill('0') << d_.dayOfMonth() << "-"
                    << std::setw(2) << std::setfill('0') << static_cast<int>(d_.month()) << "-"
                    << d_.year();
                return oss.str();
            }
        private: 
            QuantLib::Date d_;
    };

    // The period class represents a relative duration of time, has a natural number and units of length(like D, W, M, Y)
    using Period = QuantLib::Period;

    // This class stores either a Date or a Period
    class TermOrTerminationDate {
        private:
            // We prefer variant over union since it keeps track of the type it stores (union expects us to keep track externally)
            std::variant<Date, Period> val_;
            
        public:
            // Construct Date from iso string 'DD-MM-YYYY' or from tenor string like '3M', '1Y'
            explicit TermOrTerminationDate(std::string_view s) {
                 // If it contains '-', treat as date; else treat as tenor.
                if (s.find('-') != std::string_view::npos) {
                    val_ = Date(s);
                } else {
                    // QuantLib can parse tenors like "3M", "1Y" via PeriodParser (unfortunately no constructor for strings in c++)
                    val_ = Period(QuantLib::PeriodParser::parse(std::string(s)));
                }
            }

            // Constructors from Quantlib's Period or Date or our own Date 
            explicit TermOrTerminationDate(const Period& p): val_(p) {};
            explicit TermOrTerminationDate(const QuantLib::Date& d): val_(Date(d)) {};
            explicit TermOrTerminationDate(const Date& d): val_(d) {};
            
            // std::holds alternative checks if variant holds the alternative type given
            bool is_term() const { return std::holds_alternative<Period>(val_); }
            bool is_date() const { return std::holds_alternative<Date>(val_); }

            // Accessors
            const Date&   get_date() const;
            const Period& get_term() const;
        
    };
}

