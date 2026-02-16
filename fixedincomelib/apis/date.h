#pragma once

#include "fixedincomelib/Date/utilities.h"
#include "fixedincomelib/Date/basics.h"
#include "fixedincomelib/market/basics.h"

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
    // Convert QuantLib Date object to string
    std::string to_iso(const QuantLib::Date& d) {
        std::ostringstream oss;
        oss << QuantLib::io::long_date(d);
        return oss.str();
    }

    // Functions that take strings as inputs and returns a string as an output (mainly for viewing)

    std::string qfAddPeriod(std::string start_date,
                            std::string term ,
                            std::string holiday_convention = "NONE",
                            std::string business_day_convention = "NONE",
                            bool end_of_month = false) {
        Date start = Date(start_date);
        QuantLib::Period period = Period(QuantLib::PeriodParser::parse(std::string(term)));
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        QuantLib::BusinessDayConvention bdc = bdc_from_string(business_day_convention);

        QuantLib::Date end_date = add_period(start, period, cal, bdc, end_of_month);
        return to_iso(end_date);
    }

    // qfAccrued(start_date, end_date, dc="NONE", bdc="NONE", hol="NONE") -> double
    double qfAccrued(std::string start_date,
                     std::string end_date,
                     std::string accrual_basis = "NONE",
                     std::string business_day_convention = "NONE",
                     std::string holiday_convention = "NONE") {
        Date start = Date(start_date);
        Date end = Date(end_date);
        QuantLib::DayCounter dc  = accrualbasis_from_string(accrual_basis);
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        QuantLib::BusinessDayConvention bdc = bdc_from_string(business_day_convention);

        return accrued(start, end, dc, bdc, cal);
    }

    // qfMoveToBusinessDay(date, bdc, hol) -> ISO string
    std::string qfMoveToBusinessDay(std::string input_date,
                                    std::string business_day_convention,
                                    std::string holiday_convention) {
        Date d = Date(input_date);
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        QuantLib::BusinessDayConvention bdc = bdc_from_string(business_day_convention);

        QuantLib::Date moved = move_to_business_day(d, cal, bdc);
        return to_iso(moved);
    }

    // qfIsBusinessDay(date, hol) -> bool
    inline bool qfIsBusinessDay(std::string input_date,
                                std::string holiday_convention) {
        Date d = Date(input_date);
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        return is_business_day(d, cal);
    }

    // qfIsHoliday(date, hol) -> bool
    inline bool qfIsHoliday(std::string input_date,
                            std::string holiday_convention) {
        Date d = Date(input_date);
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        return is_holiday(d, cal);
    }

    // qfIsEndOfMonth(date, hol) -> bool
    inline bool qfIsEndOfMonth(std::string input_date,
                               std::string holiday_convention) {
        Date d = Date(input_date);
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        return is_end_of_month(d, cal);
    }

    // qfEndOfMonth(date, hol) -> ISO string
    inline std::string qfEndOfMonth(std::string input_date,
                                    std::string holiday_convention) {
        Date d = Date(input_date);
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        return to_iso(end_of_month(d, cal));
    }
    
    std::string qfMakeSchedule(
        std::string start_date,
        std::string end_date,
        std::string accrual_period,
        std::string holiday_convention,
        std::string business_day_convention,
        std::string accrual_basis,
        std::string rule = "BACKWARD",
        bool end_of_month = false,
        bool fix_in_arrear = false,
        std::string fixing_offset = "0D",
        std::string payment_offset = "0D",
        std::string payment_business_day_convention = "F",
        std::string payment_holiday_convention = "USGS") {

        Date s  = Date(start_date);
        Date e  = Date(end_date);

        QuantLib::Period acc_period = QuantLib::PeriodParser::parse(accrual_period);
        QuantLib::Period fix_off    = QuantLib::PeriodParser::parse(fixing_offset);
        QuantLib::Period pay_off    = QuantLib::PeriodParser::parse(payment_offset);

        QuantLib::Calendar accrualCal = calendar_from_string(holiday_convention);
        QuantLib::BusinessDayConvention accrualBdc = bdc_from_string(business_day_convention);
        QuantLib::DayCounter dc = accrualbasis_from_string(accrual_basis);

        QuantLib::Calendar payCal = calendar_from_string(payment_holiday_convention);
        QuantLib::BusinessDayConvention payBdc = bdc_from_string(payment_business_day_convention);

        std::vector<ScheduleRow> schedule = make_schedule(
            s, e, acc_period,
            accrualCal, accrualBdc, dc,
            rule,
            end_of_month,
            fix_in_arrear,
            fix_off,
            pay_off,
            payBdc,
            payCal
        );

        std::string out = "StartDate, EndDate, FixingDate, PaymentDate, Accrued\n";
        for (const auto& r : schedule) {
            out += to_iso(r.startDate) + ",";
            out += to_iso(r.endDate) + ",";
            out += to_iso(r.fixingDate) + ",";
            out += to_iso(r.paymentDate) + ",";
            out += std::to_string(r.accrued) + "\n";
        }
        return out;
    }
}

