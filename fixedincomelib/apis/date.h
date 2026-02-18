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
#include <array>
#include <algorithm>
#include <sstream>
#include <iomanip>

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

        Date end_date = add_period(start, period, cal, bdc, end_of_month);
        return end_date.get_date_str();
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

        Date moved = move_to_business_day(d, cal, bdc);
        return moved.get_date_str();
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
        return end_of_month(d, cal).get_date_str();
    }
    
    std::string qfMakeSchedule(std::string start_date,
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
        Date s(start_date);
        Date e(end_date);
    
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
    
        // ---------- aligned output (returns a string) ----------
        auto fmt_double = [](double x, int prec = 6) {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(prec) << x;
            return oss.str();
        };
    
        const std::vector<std::string> headers = {
            "StartDate","EndDate","FixingDate","PaymentDate","Accrued"
        };
    
        std::vector<std::array<std::string,5>> data;
        data.reserve(schedule.size());
    
        for (const auto& r : schedule) {
            data.push_back({
                Date(r.startDate).get_date_str(),   // "dd-mm-yyyy"
                Date(r.endDate).get_date_str(),
                Date(r.fixingDate).get_date_str(),
                Date(r.paymentDate).get_date_str(),
                fmt_double(r.accrued, 6)
            });
        }
    
        std::array<std::size_t,5> w{};
        for (int j = 0; j < 5; ++j) w[j] = headers[j].size();
        for (const auto& row : data)
            for (int j = 0; j < 5; ++j)
                w[j] = std::max(w[j], row[j].size());
    
        std::ostringstream out;
    
        // header
        for (int j = 0; j < 5; ++j)
            out << std::left << std::setw(static_cast<int>(w[j] + 2)) << headers[j];
        out << "\n";
    
        // separator
        for (int j = 0; j < 5; ++j)
            out << std::string(w[j], '-') << "  ";
        out << "\n";
    
        // rows
        for (const auto& row : data) {
            for (int j = 0; j < 5; ++j) {
                if (j == 4) out << std::right << std::setw(static_cast<int>(w[j] + 2)) << row[j];
                else        out << std::left  << std::setw(static_cast<int>(w[j] + 2)) << row[j];
            }
            out << "\n";
        }
    
        return out.str();
    }
}

