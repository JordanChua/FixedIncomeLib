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
    std::string qfAddPeriod(std::string start_date,
                            std::string term ,
                            std::string holiday_convention = "NONE",
                            std::string business_day_convention = "NONE",
                            bool end_of_month = false) {
        QuantLib::Date date = Date(start_date);
        QuantLib::Period period = Period(term);
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        QuantLib::BusinessDayConvention bdc = bdc_from_string(business_day_convention);

        QuantLib::Date end_date = add_period(date, period, cal, bdc, end_of_month);
        return QuantLib::io::long_date(end_date);
    }

    // qfAccrued(start_date, end_date, dc="NONE", bdc="NONE", hol="NONE") -> double
    double qfAccrued(std::string start_date,
                     std::string end_date,
                     std::string accrual_basis = "NONE",
                     std::string business_day_convention = "NONE",
                     std::string holiday_convention = "NONE") {
        QuantLib::Date start = Date(start_date);
        QuantLib::Date end = Date(end_date);
        QuantLib::DayCounter dc  = accrualbasis_from_string(accrual_basis);
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        QuantLib::BusinessDayConvention bdc = bdc_from_string(business_day_convention);

        return accrued(start, end, dc, cal, bdc);
    }

    // qfMoveToBusinessDay(date, bdc, hol) -> ISO string
    std::string qfMoveToBusinessDay(std::string input_date,
                                    std::string business_day_convention,
                                    std::string holiday_convention) {
        QuantLib::Date d = Date(input_date);;
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        QuantLib::BusinessDayConvention bdc = bdc_from_string(business_day_convention);

        QuantLib::Date moved = move_to_business_day(d, cal, bdc);
        return QuantLib::io::long_date(moved);
    }

    // qfIsBusinessDay(date, hol) -> bool
    inline bool qfIsBusinessDay(std::string input_date,
                                std::string holiday_convention) {
        QuantLib::Date d = Date(input_date);
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        return cal.isBusinessDay(d);
    }

    // qfIsHoliday(date, hol) -> bool
    inline bool qfIsHoliday(std::string input_date,
                            std::string holiday_convention) {
        QuantLib::Date d = Date(input_date);
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        return cal.isHoliday(d);
    }

    // qfIsEndOfMonth(date, hol) -> bool
    inline bool qfIsEndOfMonth(std::string input_date,
                               std::string holiday_convention) {
        QuantLib::Date d = Date(input_date);
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        return cal.isEndOfMonth(d);
    }

    // qfEndOfMonth(date, hol) -> ISO string
    inline std::string qfEndOfMonth(std::string input_date,
                                    std::string holiday_convention) {
        QuantLib::Date d = Date(input_date);
        QuantLib::Calendar cal = calendar_from_string(holiday_convention);
        return QuantLib::io::long_date(cal.endOfMonth(d));
    }
    
    std::vector<ScheduleRow> qfCreateSchedule(
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

        QuantLib::Date s  = Date(start_date);
        QuantLib::Date e  = Date(end_date);

        QuantLib::Period acc_period = Period(accrual_period);
        QuantLib::Period fix_off = Period(fixing_offset);
        QuantLib::Period pay_off = Period(payment_offset);

        QuantLib::Calendar accrualCal = calendar_from_string(holiday_convention);
        QuantLib::BusinessDayConvention accrualBdc = bdc_from_string(business_day_convention);
        QuantLib::DayCounter dc = daycounter_from_string(accrual_basis);

        QuantLib::Calendar payCal = calendar_from_string(payment_holiday_convention);
        QuantLib::BusinessDayConvention payBdc = bdc_from_string(payment_business_day_convention);

        vector<ScheduleRow> schedule = make_schedule(
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
        for (const auto& r : rows) {
            out += to_iso(r.startDate) + ",";
            out += to_iso(r.endDate) + ",";
            out += to_iso(r.fixingDate) + ",";
            out += to_iso(r.paymentDate) + ",";
            out += std::to_string(r.accrued) + "\n";
        }
        return out;
    }
}

