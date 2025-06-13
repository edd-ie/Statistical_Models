//
// Created by _edd.ie_ on 13/06/2025.
//

#include "Bond.h"


Bond::Bond(const std::string& bond_id, const Date& dated_date,
    const Date& first_coupon_date,    const Date& penultimate_coupon_date,
    const Date& maturity_date, int coupon_frequency, double coupon_rate,
    double face_value) : bond_id_{bond_id}{
    //  Number of months in coupon period:
    const int months_in_regular_coupon_period = 12 / coupon_frequency;

    //  Regular coupon payment:
    const double regular_coupon_payment = coupon_rate
        * face_value / coupon_frequency;

    calculate_pmt_schedule_(first_coupon_date, penultimate_coupon_date,
        months_in_regular_coupon_period, regular_coupon_payment);

    amend_initial_irregular_dates_and_pmts_(dated_date, first_coupon_date,
        months_in_regular_coupon_period, regular_coupon_payment);

    amend_final_irregular_dates_and_pmts_(penultimate_coupon_date, maturity_date,
        months_in_regular_coupon_period, regular_coupon_payment, face_value);

    //  Maturity date is a due date that falls on a business day
    due_dates_.push_back(maturity_date);
    payment_dates_.push_back(maturity_date);
}


void Bond::calculate_pmt_schedule_(const Date& first_coupon_date,
    const Date& penultimate_coupon_date,
    int months_in_regular_coupon_period, double regular_coupon_payment)
{
    //  Generate vectors containing due dates, payment dates,
    // and regular coupon payment amounts:
    for (Date regular_due_date{first_coupon_date};
        regular_due_date <= penultimate_coupon_date;
        regular_due_date.addMonths(months_in_regular_coupon_period))
    {
        // The due and payment Dates
        due_dates_.push_back(regular_due_date);
        Date payment_date{regular_due_date};
        // Roll any due dates falling on a weekend and
        // store as payment dates:
        payment_dates_.push_back(payment_date.weekend_roll());
        // Assume all coupons are regular;
        // deal with any irregular first or last periods later:
        payment_amounts_.push_back(regular_coupon_payment);
    }
}
void Bond::amend_initial_irregular_dates_and_pmts_(const Date& dated_date,
   const Date& first_coupon_date,
   const int months_in_regular_coupon_period,
   const double regular_coupon_payment)
{
    //  If first coupon is irregular, amend the coupon payment
    Date first_prior{first_coupon_date};
    first_prior.addMonths(-months_in_regular_coupon_period);
    if (first_prior != dated_date) // if true then irregular coupon
    {
        if (first_prior < dated_date) // if true then short coupon period
        {
            double coupon_fraction = static_cast<double>(first_coupon_date - dated_date) /
                static_cast<double>(first_coupon_date - first_prior);
            payment_amounts_[0] *= coupon_fraction;
        }
        else // dated_date < first_prior, so long coupon period
        {
            // long_first_coupon = regular_coupon + extra_interest
            // Calculate the second_prior,
            // the last regular date before the first_prior:
            Date second_prior{first_prior};
            second_prior.addMonths(-months_in_regular_coupon_period);
            double coupon_fraction =
                static_cast<double>(first_prior - dated_date) /
                static_cast<double>(first_prior - second_prior);
            payment_amounts_[0] += coupon_fraction * regular_coupon_payment;
        }
    }
 }

void Bond::amend_final_irregular_dates_and_pmts_(const Date& penultimate_coupon_date,
    const Date& maturity_date, const int months_in_regular_coupon_period,
    const double regular_coupon_payment, double face_value){
    //  If final coupon period is irregular, amend the coupon payment.
    Date maturity_regular_date{penultimate_coupon_date};
    //  Calculate maturity_regular_date, the first regular date
    // after penultimate_coupon_date:
    maturity_regular_date.addMonths(months_in_regular_coupon_period);
    double final_coupon{regular_coupon_payment};
    // If true then irregular coupon period
    if (maturity_regular_date != maturity_date)
    {
        // If true, adjust for short coupon period:
        if (maturity_date < maturity_regular_date)
        {
            double coupon_fraction =
                static_cast<double>(maturity_date - penultimate_coupon_date)
                / static_cast<double>(
                    maturity_regular_date - penultimate_coupon_date);
            final_coupon *= coupon_fraction;
        }
        // maturity_regular_date < maturity_date,
        // so have long coupon period adjustment:
        else
        {
            // final_coupon = regular_coupon_amount + extra_interest.
            // Calculate the next_regular_date, the first regular date
            // after the maturity_regular_date:
            Date next_regular_date{maturity_regular_date};
            next_regular_date.addMonths(months_in_regular_coupon_period);
            double extra_coupon_fraction =
                static_cast<double>(maturity_date - maturity_regular_date) /
                static_cast<double>(next_regular_date - maturity_regular_date);
            final_coupon += extra_coupon_fraction * regular_coupon_payment;
        }
    }
    //  Calculate final payment:
    payment_amounts_.push_back(face_value + final_coupon);
}


double Bond::discounted_value(const Date& bond_settle_date, const YieldCurve& yield_curve){
    // The buyer receives the payments which fall due after the bond_settle_date.
    // If the bond_settle_date falls on a due_date,
    // the seller receives the payment:
    double pv = 0.0;
    for (size_t i = 0; i < due_dates_.size(); i++)
    {
        if (bond_settle_date < due_dates_[i])
        {
            pv += yield_curve.discountFactor(bond_settle_date,
                payment_dates_[i]) * payment_amounts_[i];
        }
    }
    return yield_curve.discountFactor(yield_curve.getSettleDate(),
        bond_settle_date) * pv;
}