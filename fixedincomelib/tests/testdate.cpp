
#include <iostream>
#include <vector>

#include <ql/time/date.hpp>
#include <ql/time/period.hpp>

// your library headers (adjust paths to match your project)
#include "fixedincomelib/apis/date.h"
// #include "fixedincomelib/date_utils.hpp"       // add_period, move_to_business_day, accrued, etc.
// #include "fixedincomelib/schedule_utils.hpp"   // make_schedule + CashflowRow

int main() {
    using namespace QuantLib;
    using namespace fixedincomelib;

    try {
        std::cout << "=== Test All Kinds of Date Functions ===\n";

        // --------- 1) AddPeriod -----------
        std::string start_date = "25-05-2025";
        std::string term       = "3M";
        std::string bdc        = "F";
        std::string hol        = "USGS";
        bool end_of_month      = false;

        std::string end_date = qfAddPeriod(start_date, term, hol, bdc, end_of_month);

        std::cout << "\n[AddPeriod]\n";
        std::cout << "start_date=" << start_date << " term=" << term
                  << " => end_date=" << end_date << "\n";

        // --------- 2) Accrued -----------
        std::string accrual_basis = "ACT/ACT";
        double yf = qfAccrued(start_date, end_date, accrual_basis, bdc, hol);

        std::cout << "\n[Accrued]\n";
        std::cout << "start=" << start_date << " end=" << end_date
                  << " dc=" << accrual_basis << " => yearFraction=" << yf << "\n";

        // --------- 3) MoveToBusinessDay -----------
        std::string test_date = "21-12-2025";
        std::string moved = qfMoveToBusinessDay(test_date, bdc, hol);

        std::cout << "\n[MoveToBusinessDay]\n";
        std::cout << "input=" << test_date << " => adjusted=" << moved << "\n";

        // --------- 4) IsBusinessDay -----------
        std::cout << "\n[IsBusinessDay]\n";
        std::cout << test_date << " => " << (qfIsBusinessDay(test_date, hol) ? "true" : "false") << "\n";

        // --------- 5) IsHoliday -----------
        std::string holiday_date = "01-01-2026";
        std::cout << "\n[IsHoliday]\n";
        std::cout << holiday_date << " => " << (qfIsHoliday(holiday_date, hol) ? "true" : "false") << "\n";

        // --------- 6) IsEndOfMonth -----------
        std::string eom_test1 = "21-12-2025";
        std::string eom_test2 = "31-12-2025";
        std::cout << "\n[IsEndOfMonth]\n";
        std::cout << eom_test1 << " => " << (qfIsEndOfMonth(eom_test1, hol) ? "true" : "false") << "\n";
        std::cout << eom_test2 << " => " << (qfIsEndOfMonth(eom_test2, hol) ? "true" : "false") << "\n";

        // --------- 7) EndOfMonth -----------
        std::string eom_input = "01-01-2025"
        std::string eom = qfEndOfMonth(eom_input)
        std::cout << "\n[EndOfMonth]\n";
        std::cout << eom_input << " => " << eom << "\n";

        // --------- 8) CreateSchedule -----------
        std::string sched_start = "25-05-2025";
        std::string sched_end   = "30-01-2027";

        std::string acc_period = "6M";  
        std::string acc_basis = "ACT/360";
        DayCounter sched_dc = daycounter_from_string(sched_dc_s);

        std::string rule_s = "BACKWARD";
        bool sched_eom = false;
        bool fix_in_arrear = true;

        Period fixing_offset("1D");
        Period payment_offset("2D");

        // payment conventions
        BusinessDayConvention pay_bdc = bdc_from_string("F");
        Calendar pay_cal = calendar_from_string("USGS");

        auto rows = make_schedule(
            sched_start,
            sched_end,
            accrual_period,
            cal,                 // accrual holiday convention
            bdc,                 // accrual business day convention
            sched_dc,            // accrual basis
            rule_s,
            sched_eom,
            fix_in_arrear,
            fixing_offset,
            payment_offset,
            pay_bdc,
            pay_cal
        );

        std::cout << "\n[CreateSchedule]\n";
        std::cout << "start=" << sched_start << " end=" << sched_end
                  << " period=" << accrual_period << " dc=" << sched_dc_s
                  << " rule=" << rule_s << " fix_in_arrear=" << (fix_in_arrear ? "true" : "false")
                  << " fixing_offset=" << fixing_offset << " payment_offset=" << payment_offset
                  << "\n";

        std::cout << "Rows: " << rows.size() << "\n";
        std::cout << "StartDate, EndDate, FixingDate, PaymentDate, Accrued\n";
        for (const auto& r : rows) {
            std::cout << r.startDate << ", "
                      << r.endDate << ", "
                      << r.fixingDate << ", "
                      << r.paymentDate << ", "
                      << r.accrued << "\n";
        }

        std::cout << "\nAll tests completed.\n";
        return 0;

    } catch (const std::exception& ex) {
        std::cerr << "ERROR: " << ex.what() << "\n";
        return 1;
    }
}