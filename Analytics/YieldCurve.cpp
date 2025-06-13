//
// Created by _edd.ie_ on 13/06/2025.
//

#include "YieldCurve.h"

Date YieldCurve::getSettleDate() const{
    return settleDate;
}
Act365 YieldCurve::getAct365() const{
    return act365;
}


double YieldCurve::discountFactor(const Date& date1, const Date& date2) const {
    if (date2 < date1)    {
        throw std::invalid_argument{
            "discount_factor(.): invalid inequality: date2 < date1"};
    }
    if (date1 < getSettleDate() || date2 < getSettleDate())    {
        throw std::invalid_argument{
            "discount_factor(.): dates must fall on or after settle date"};
    }
    if (date1 == date2)    {
        return 1.0;        // exp(0.0)
    }

    // P(t1, t2) = exp( -(t2-t1) * f(t1, t2) )
    double t2 = getAct365().year_fraction(getSettleDate(), date2);
    double y2 = yieldCurve(t2);

    // if d1 == settle_ then P(t1,t2) = P(0,t2) = exp(-t2 * y2 )
    if (date1 == getSettleDate())    {
        return exp(-t2 * y2);
    }
    
    double t1 = getAct365().year_fraction(getSettleDate(), date1);;
    double y1 = yieldCurve(t1);

    // (t2-t1) f(t1,t2) = t2 * y2 - t1 * y1
    return exp(t1 * y1 - t2 * y2);
}



LinearInterpYieldCurve::LinearInterpYieldCurve(const Date& settle_date,
    const std::vector<Date>& maturity_dates, const std::vector<double>& unit_prices) :
YieldCurve{settle_date}{
    using std::size_t;
    if (maturity_dates.size() != unit_prices.size())
        throw std::invalid_argument{
            "Maturity_dates and spot_discount_factors different lengths"};
    if (maturity_dates.front() < this->getSettleDate())
        throw std::invalid_argument{"First maturity date before settle date"};
    // Prevent vector memory reallocation (reserve(.)):
    maturities.reserve(maturity_dates.size());
    yields.reserve(maturity_dates.size());

    // Assume maturity dates in are in ascending order
    for (size_t i = 0; i < maturity_dates.size(); i++)
    {
        double t = getAct365().year_fraction(this->getSettleDate(), maturity_dates[i]);
        maturities.push_back(t);
        yields.push_back(-std::log(unit_prices[i]) / t);
    }
}


double LinearInterpYieldCurve::yieldCurve(double t) const
{
    // interp_yield called from discount_factor, so maturities_front() <= t
    using std::size_t;

    if (t >= maturities.back()) {
        return yields.back();
    }

    // We now know maturities_front() <= t < maturities_.back()
    size_t indx = 0;
    while (maturities[indx + 1] < t){
        ++indx;
    }

    return yields[indx] + (yields[indx + 1] - yields[indx])
        / (maturities[indx + 1] - maturities[indx]) * (t - maturities[indx]);
}