//
// Created by _edd.ie_ on 12/06/2025.
//

#ifndef DATE_H
#define DATE_H

#include <chrono>
#include <compare>



class Date {
    std::chrono::year_month_day date{};
    void validate() const;
public:
    Date(int year, unsigned month, unsigned day);
    Date(std::chrono::year_month_day date);
    Date() = default;

    // Accessors
    int year() const;
    unsigned day() const;
    unsigned month() const;
    int serialDate() const;
    std::chrono::year_month_day ymd() const;

    // Properties
    unsigned days_in_month() const;
    bool is_end_of_month() const;
    bool is_leap_year() const;
    
    // Operators
    int operator - (const Date& rhs) const;
    bool operator == (const Date& rhs) const;
    std::strong_ordering operator <=> (const Date& rhs) const;

    // Modifying member functions:
    Date& addYears(int years);
    Date& addMonths(int months);
    Date& addDays(int days);
    Date& weekend_roll();
};



/**
 * Convert the interval between two dates into time measured in years
 * usually referred to as a year fraction for shorter intervals.
 *
 * US & EU - Actual/360 = normal days in a month, 360 days in a year
 * UK, Canadian & Australian - Actual/365 = normal days in a month, 365 days in a year
 * 30/360 = 30 days in a month, 360 days in a year
 * Equity portfolio management - Actual/252 = normal days in a month, 252 days in a year
 *
**/



class DayCount {
public:
    virtual double year_fraction (const Date& date_01, const Date& date_02) const = 0;
    virtual ~DayCount() = default;
};


class Act365 : public DayCount{
public:
    double year_fraction(const Date& date_01, const Date& date_02) const override;
};


class Act360 : public DayCount{
public:
    double year_fraction(const Date& date_01, const Date& date_02) const override;
};


class Thirty360 : public DayCount{
    int date_diff_(const Date& date_01, const Date& date_02) const;
public:
    double year_fraction(const Date& date_01, const Date& date_02) const override;
};

#endif //DATE_H
