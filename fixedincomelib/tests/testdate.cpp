
#include <iostream>
#include <vector>

#include <ql/time/date.hpp>
#include <ql/time/period.hpp>

// your library headers (adjust paths to match your project)
#include "fixedincomelib/apis/date.h"
// #include "fixedincomelib/Date/basics.h"


int main() {
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
        std::cout << "start_date = " << start_date << " term = " << term
                  << " => end_date = " << end_date << "\n";

        // --------- 2) Accrued -----------
        std::string accrual_basis = "ACT/ACT";
        std::string acc_end_date = "25-08-2025";
        double yf = qfAccrued(start_date, acc_end_date, accrual_basis, bdc, hol);

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
        std::string eom_input = "01-01-2025";
        std::string eom = qfEndOfMonth(eom_input, hol);
        std::cout << "\n[EndOfMonth]\n";
        std::cout << eom_input << " => " << eom << "\n";

        // --------- 8) CreateSchedule -----------
        std::string sched_start = "25-05-2025";
        std::string sched_end   = "30-01-2027";

        // Accrual details + conventions
        std::string acc_period = "6M";  
        std::string acc_basis = "ACT/360";
        std::string acc_hol = "USGS";
        std::string acc_bdc = "F";
        
        // Rule + EOM and fix in arrear
        std::string rule = "BACKWARD";
        bool sched_eom = false;
        bool fix_in_arrear = true;

        // Offsets + payment conventions
        std::string fixing_offset = "1D";
        std::string payment_offset = "2D";
        std::string pay_bdc = "F";
        std::string pay_cal = "USGS";

        std::string output = qfMakeSchedule(
            sched_start,
            sched_end,
            acc_period,
            acc_hol,                // accrual holiday convention
            acc_bdc,                // accrual business day convention
            acc_basis,              // accrual basis
            rule,
            sched_eom,
            fix_in_arrear,
            fixing_offset,
            payment_offset,
            pay_bdc,
            pay_cal
        );

        std::cout << "\n[CreateSchedule]\n";
        std::cout << "start=" << sched_start << " end=" << sched_end
                  << " period=" << acc_period << " acc_basis =" << acc_basis
                  << " rule=" << rule << " fix_in_arrear=" << (fix_in_arrear ? "true" : "false")
                  << " fixing_offset=" << fixing_offset << " payment_offset=" << payment_offset
                  << "\n";

        std::cout << output << "\n";

        std::cout << "\nAll tests completed.\n";
        return 0;

    } catch (const std::exception& ex) {
        std::cerr << "ERROR: " << ex.what() << "\n";
        return 1;
    }
}

/* 
Output of this test file 

=== Test All Kinds of Date Functions ===

[AddPeriod]
start_date = 25-05-2025 term = 3M => end_date = August 25th, 2025

[Accrued]
start=25-05-2025 end=August 25th, 2025 dc=ACT/ACT => yearFraction=0.252055

[MoveToBusinessDay]
input=21-12-2025 => adjusted=December 22nd, 2025

[IsBusinessDay]
21-12-2025 => false

[IsHoliday]
01-01-2026 => true

[IsEndOfMonth]
21-12-2025 => false
31-12-2025 => true

[EndOfMonth]
01-01-2025 => January 31st, 2025

[CreateSchedule]
start=25-05-2025 end=30-01-2027 period=6M acc_basis =ACT/360 rule=BACKWARD fix_in_arrear=true fixing_offset=1D payment_offset=2D
StartDate, EndDate, FixingDate, PaymentDate, Accrued
May 27th, 2025,July 30th, 2025,July 31st, 2025,August 1st, 2025,0.177778
July 30th, 2025,January 30th, 2026,February 2nd, 2026,February 3rd, 2026,0.511111
January 30th, 2026,July 30th, 2026,July 31st, 2026,August 3rd, 2026,0.502778
July 30th, 2026,February 1st, 2027,February 2nd, 2027,February 3rd, 2027,0.516667


All tests completed.
*/