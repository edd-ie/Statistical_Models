//
// Created by _edd.ie_ on 13/06/2025.
//

#ifndef YIELDCURVE_H
#define YIELDCURVE_H

#include "../Utilities/Date.h"

#include <vector>
#include <stdexcept>
#include <cmath>

using std::vector;
using std::exp;

class YieldCurve {
    Date settleDate;
    virtual double yieldCurve(double t) const = 0;
    inline static Act365 act365{};    // The yields are continuously compounded
                                        // with Actual/365 day count basis

protected:
    YieldCurve(Date settleDate) : settleDate{std::move(settleDate)} {}
    Act365 getAct365() const;

public:
    virtual ~YieldCurve() = default;
    double discountFactor(const Date& date1, const Date& date2) const;
    Date getSettleDate() const;
};


class CubicSplineYieldCurve : public YieldCurve {
    vector<double> maturities;
    vector<double> yields;
    double yieldCurve(double t) const override;

public:
    CubicSplineYieldCurve();
};

class MonotoneCVxSplineYieldCurve : public YieldCurve {
    vector<double> maturities;
    vector<double> yields;
    double yieldCurve(double t) const override;

public:
    MonotoneCVxSplineYieldCurve();
};

class LinearInterpYieldCurve final: public YieldCurve {
    vector<double> maturities;
    vector<double> yields;
    double yieldCurve(double t) const override;

public:
    LinearInterpYieldCurve(const Date& settle_date,  const std::vector<Date>& maturity_dates,
        const std::vector<double>& unit_prices);
};

class QuadraticFwdSplineYieldCurve : public YieldCurve {
    vector<double> maturities;
    vector<double> yields;
    double yieldCurve(double t) const override;

public:
    QuadraticFwdSplineYieldCurve();
};


#endif //YIELDCURVE_H
