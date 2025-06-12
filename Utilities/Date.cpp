//
// Created by _edd.ie_ on 12/06/2025.
//

#include "Date.h"

Date::Date(int year, unsigned month, unsigned day):
    date{std::chrono::year{year}/std::chrono::month{month}/std::chrono::day{day}} {
    validate();
}

void Date::validate() const{
    if (!date.ok())    // std::chrono member function to check if valid date
        throw std::invalid_argument{"Date constructor: Invalid date."};
}

Date::Date(std::chrono::year_month_day date) : date{std::move(date)}{
    validate();
}

int Date::serialDate() const{
    return std::chrono::sys_days(date).time_since_epoch().count();
}

std::chrono::year_month_day Date::ymd() const{
    return date;
}

int Date::year() const{
    return static_cast<int>(date.year());
}

unsigned Date::month() const{
    return static_cast<unsigned>(date.month());
}

unsigned Date::day() const{
    return static_cast<unsigned>(date.day());
}

unsigned Date::days_in_month() const{
    using namespace std::chrono;
    year_month_day_last eom{date.year() / date.month() / last};
    return static_cast<unsigned>(eom.day());
}

bool Date::is_end_of_month() const{
    return date == date.year() / date.month() / std::chrono::last;
}

bool Date::is_leap_year() const{
    return date.year().is_leap();
}

int Date::operator -(const Date& rhs) const{
    return this->serialDate() - rhs.serialDate();
}

bool Date::operator == (const Date& rhs) const{
    return this->ymd() == rhs.ymd();
}

std::strong_ordering Date::operator <=> (const Date& rhs) const{
    return this->ymd() <=> rhs.ymd();
}

Date& Date::addYears(const int years) {
    // Proceed naively:
    date += std::chrono::years(years);
    // The Only possible error case is if the month is February
    // and the result is day = 29 in a non-leap year.
    if (!date.ok())
        date = date.year() / date.month() / 28;
    
    return *this;
}

Date& Date::addMonths(const int months) {
    date += std::chrono::months(months);
    if (!date.ok())
        date = date.year() / date.month() / std::chrono::day{days_in_month()};
    return *this;
}

Date& Date::addDays(const int days){
    date = std::chrono::sys_days(date) + std::chrono::days(days);
    return *this;
}

Date& Date::weekend_roll() {
    using namespace std::chrono;

    const weekday wd{sys_days(date)};        // std::chrono::weekday
    std::chrono::month orig_mth{date.month()};

    // Sat: 8-6 = 2 days to roll forward, Sun: 8-7 = 1-day roll forward
    if (unsigned wdn{wd.iso_encoding()}; wdn > 5)
        date = sys_days(date) + days(8 - wdn);

    // Case where date gets rolled into the 1st Monday of the next month -
    // Modified Following rule says to roll back three days to the previous biz day:
    if (orig_mth != date.month())
        date = sys_days(date) - days(3);

    return *this;
}

std::ostream& operator << (std::ostream& os, const Date& rhs){
    os << rhs.ymd();
    return os;
}



/** DayCount basis **/
// t = (d2-d1)/365
double Act365::year_fraction(const Date& date_01, const Date& date_02) const {
    return (date_02 - date_01) / 365.0;
}

// t = (d2-d1)/360
double Act360::year_fraction(const Date& date_01, const Date& date_02) const {
    return (date_02 - date_01) / 360.0;
}

// t = [360(y2-y1) + 30(m2-m1) + (d2-d1)]/360
int Thirty360::date_diff_(const Date& date_01,const Date& date_02) const{
    int d1, d2;
    d1 = date_01.day();
    d2 = date_02.day();

    if (d1 == 31) d1 = 30;
    if ((d2 == 31) && (d1 == 30)) d2 = 30;

    return 360 * (date_02.year() - date_01.year()) + 30 *
        (date_02.month() - date_01.month()) + d2 - d1;
}

double Thirty360::year_fraction(const Date& date_01, const Date& date_02) const{
    return date_diff_(date_01, date_02) / 360.0;
}
