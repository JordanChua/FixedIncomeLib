// Refer to https://www.quantlib.org/reference/class_quant_lib_1_1_calendar.html for documentation 
#pragma once

#include "fixedincomelib/Date/basics.h"

#include <ql/time/date.hpp>
#include <ql/time/period.hpp>
#include <ql/time/schedule.hpp>
#include <ql/time/calendars/unitedstates.hpp>
#include <ql/time/daycounters/actualactual.hpp>
#include <ql/time/businessdayconvention.hpp>
#include <vector>
#include <string>
#include <string_view>

namespace fixedincomelib {

    struct ScheduleRow {
        QuantLib::Date startDate;
        QuantLib::Date endDate;
        QuantLib::Date fixingDate;
        QuantLib::Date paymentDate;
        double accrued;
    };
    
    // add_period: calendar.advance(start, term, bdc, endOfMonth)
    Date add_period(const Date& start_date,
                              const Period& term,
                              const QuantLib::Calendar& cal = QuantLib::UnitedStates(QuantLib::UnitedStates::FederalReserve),
                              QuantLib::BusinessDayConvention bdc = QuantLib::Following,
                              bool end_of_month = false) {
        return Date(cal.advance(start_date.get_date(), term, bdc, end_of_month));
    }
    
    // move_to_business_day: calendar.adjust(date, bdc)
    Date move_to_business_day(const Date& input_date,
                                        const QuantLib::Calendar& cal,
                                        const QuantLib::BusinessDayConvention bdc) {
        return Date(cal.adjust(input_date.get_date(), bdc));
    }
    
    // accrued: dayCounter.yearFraction(start, adjusted_end)
    double accrued(const Date& start_date,
                   const Date& end_date,
                   const QuantLib::DayCounter& dc,
                   QuantLib::BusinessDayConvention bdc = QuantLib::Following,
                   const QuantLib::Calendar& cal = QuantLib::UnitedStates(QuantLib::UnitedStates::FederalReserve)) {
        Date adjusted_end = move_to_business_day(end_date, cal, bdc);
        return dc.yearFraction(start_date.get_date(), adjusted_end.get_date());
    }
    
    bool is_business_day(const Date& d, const QuantLib::Calendar& cal) {
        return cal.isBusinessDay(d.get_date());
    }
    bool is_holiday(const Date& d, const QuantLib::Calendar& cal) {
        return cal.isHoliday(d.get_date());
    }
    bool is_end_of_month(const Date& d, const QuantLib::Calendar& cal) {
        return cal.isEndOfMonth(d.get_date());
    }
    Date end_of_month(const Date& d, const QuantLib::Calendar& cal) {
        return Date(cal.endOfMonth(d.get_date()));
    }

    std::vector<ScheduleRow> make_schedule(
        const Date& start_date,
        const Date& end_date,
        const QuantLib::Period& accrual_period,
        const QuantLib::Calendar& holiday_convention,
        QuantLib::BusinessDayConvention business_day_convention,
        const QuantLib::DayCounter& accrual_basis,
        const std::string rule = "BACKWARD",
        bool end_of_month = false,
        bool fix_in_arrear = false,
        QuantLib::Period fixing_offset = QuantLib::Period(0, QuantLib::Days),
        QuantLib::Period payment_offset = QuantLib::Period(0, QuantLib::Days),
        QuantLib::BusinessDayConvention payment_business_day_convention = QuantLib::Following,
        const QuantLib::Calendar& payment_holiday_convention = QuantLib::UnitedStates(QuantLib::UnitedStates::FederalReserve)
    ) {
        // Rule for date generation is an enum, backward is from termination date to effective date (forward is reversed order)
        QuantLib::DateGeneration::Rule ql_rule = (rule == "BACKWARD" ? QuantLib::DateGeneration::Backward : QuantLib::DateGeneration::Forward);

        // Initialise our schedule using given parameters
        QuantLib::Schedule sched(
            start_date.get_date(),
            end_date.get_date(),
            accrual_period,
            holiday_convention,
            business_day_convention,
            business_day_convention,
            ql_rule,
            end_of_month
        ); 
    
        // Just to check that there are at least 2 dates generated in our schedule 
        const std::vector<QuantLib::Date> dates = sched.dates();
        if (dates.size() < 2) return {};

        // Reserve space to print the schedules 
        std::vector<ScheduleRow> out;
        out.reserve(dates.size() - 1);

        // start_dates = dates[:-1], end_dates = dates[1:]
        for (std::size_t i = 0; i + 1 < dates.size(); ++i) {
            const QuantLib::Date s = dates[i];
            const QuantLib::Date e = dates[i + 1];

            // Here our fix-in-arrear indicates whether the fixing is determined at the start/end of an accrual period 
            // Especially applicable for SOFR swaps where the accrual is determined towards the end
            QuantLib::Date f = s;
            if (fixing_offset.length() != 0) {
                const QuantLib::Date anchor = fix_in_arrear ? e : s;
                f = holiday_convention.advance(anchor, fixing_offset, business_day_convention, end_of_month);
            }

            // To account for payment offset 
            QuantLib::Date p = e;
            if (payment_offset.length() != 0) {
                p = payment_holiday_convention.advance(e, payment_offset, payment_business_day_convention, end_of_month);
            }

            // Compute accrued year fraction 
            double acc = accrued(Date(s), Date(e),  accrual_basis, business_day_convention, holiday_convention);
            out.push_back(ScheduleRow{ s, e, f, p, acc });
        }

        return out;
    }
    
}
