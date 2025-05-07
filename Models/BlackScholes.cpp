//
// Created by _edd.ie_ on 07/05/2025.
//

#include "BlackScholes.h"

#include <cmath>
#include <numbers>
#include <algorithm>

double BlackScholes::operator()(double vol) {
    using std::exp;
    const int phi =  static_cast<int>(payOffType);

    if (expiryTime > 0.0) {
        auto normArgs = computeNormArgs(vol);
        const double d1 = normArgs[0];
        const double d2 = normArgs[1];

        auto normCDF = [](double x)->double {
            return (1.0 + std::erf(x/std::numbers::sqrt2))/2.0;
        };

        double nD1 = normCDF(phi*d1);
        double nD2 = normCDF(phi*d2);
        double discountFactor = exp(-interestRate*expiryTime);
        return phi * (spotPrice*exp(-dividend*expiryTime))*nD1 - discountFactor
            * strikePrice * nD2;
    }
    return std::max(phi*(spotPrice-strikePrice), 0.0);
}

/**
*   d1 =  [log(S/X) + (r−q+ 1/2*σ^2) (T −t)]/[σ*sqrt(T−t)]
*   d2 = d1 - [σ*sqrt(T−t)]
*/
std::array<double, 2> BlackScholes::computeNormArgs(double volatility) {
    double div = volatility*sqrt(expiryTime);
    double d1 = (log(spotPrice/strikePrice) + (interestRate - dividend + ((volatility*volatility)/2))*expiryTime)/
        div;
    double d2 = d1 - div;

    return std::array{d1, d2};
}

double BlackScholes::impliedVolatility(BlackScholes &bsc, double marketPrice, double x0, double x1, double total, unsigned maxIteration) {
    auto diff = [ &bsc, marketPrice](double x) {
        return bsc(x)-marketPrice;
    };

    double y0 =  diff(x0),
        y1 = diff(x1),
        impliedVolatility = 0.0;
    unsigned counter = 0;

    for (counter =0; counter <= maxIteration; ++counter) {
        // x0, x1: store previous and updated volatility values
        // through each iteration.
        // y0, y1: store previous and updated values of
        // BSc opt price - opt mkt price.
        if (std::abs(x1 - x0) > total)
        {
            impliedVolatility = x1 - (x1 - x0) * y1 / (y1 - y0);
            // Update x1 & x0:
            x0 = x1;
            x1 = impliedVolatility;
            y0 = y1;
            // Use a lambda expression instead of y1 = bsc(x1) - opt_mkt_price:
            y1 = diff(x1);
        }
        else
        {
            return x1;        //  (convergence)
        }
    }

    return std::nan(""); // doesn't converge
}


