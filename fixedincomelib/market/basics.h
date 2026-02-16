#pragma once

#include <ql/currency.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/businessdayconvention.hpp>
#include <ql/time/daycounter.hpp>

#include <string>
#include <string_view>

namespace fixedincomelib {

    QuantLib::Currency currency_from_string(std::string_view ccy);
    std::string get_currency_code(const QuantLib::Currency& ccy);

    QuantLib::BusinessDayConvention bdc_from_string(std::string_view s = "NONE");
    std::string bdc_to_string(QuantLib::BusinessDayConvention bdc);

    QuantLib::Calendar calendar_from_string(std::string_view s = "NONE");

    QuantLib::DayCounter accrualbasis_from_string(std::string_view s = "NONE");

}