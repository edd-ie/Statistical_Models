//
// Created by _edd.ie_ on 13/06/2025.
//

#ifndef BOND_H
#define BOND_H


/**
 * Bond pays fixed amounts on dates in a regular schedule
 * regular coupon amount = coupon rate *  face value/coupon frequency
 *
 * Dated date - date on which interest begins to accrue.
 * Issue date - the date on which the bond goes on sale
 * First coupon date - date for 1st stream of fixed payments
 * Penultimate coupon date - date for second-to-last payment
 * Maturity date - date for final payment, consisting of the last coupon payment + repayment of the face value
 *
 * Assumptions:
 * The issue & dated date are the same.
 * Each coupon day is strictly less than 29.
 * No holidays other than Saturdays & Sundays.
*/


#include "../Utilities/Date.h"
#include "../Analytics/YieldCurve.h"

#include <cmath>
#include <vector>
#include <string>
#include <iterator>


class Bond{
    std::string bond_id_;

    // Dates on which payments are due, whether business days or not:
    std::vector<Date> due_dates_;

    // Business dates on which payments are made:
    std::vector<Date> payment_dates_;

    // Coupon and redemption payments,
    // assume redemption_payment = face_value:
    std::vector<double> payment_amounts_;

    void calculate_pmt_schedule_(const Date& first_coupon_date,
        const Date& penultimate_coupon_date,
        int months_in_regular_coupon_period, double regular_coupon_payment);

    void amend_initial_irregular_dates_and_pmts_(const Date& dated_date,
        const Date& first_coupon_date,
        const int months_in_regular_coupon_period,
        const double regular_coupon_payment);

    void amend_final_irregular_dates_and_pmts_(
        const Date& penultimate_coupon_date,
        const Date& maturity_date,
        const int months_in_regular_coupon_period,
        const double regular_coupon_payment, double face_value);

public:
    Bond(const std::string& bond_id, const Date& dated_date,
        const Date& first_coupon_date,
        const Date& penultimate_coupon_date,
        const Date& maturity_date,
        int coupon_frequency, double coupon_rate, double face_value);

    double discounted_value(const Date& bond_settle_date,
        const YieldCurve& yield_curve);

    std::string bond_id() const;
};


#endif //BOND_H
