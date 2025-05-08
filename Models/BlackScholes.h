//
// Created by _edd.ie_ on 07/05/2025.
//

#ifndef BLACKSCHOLES_H
#define BLACKSCHOLES_H
#include <array>

/**
* Black-Scholes pricing formula
* applies to calculating the value V of an (equity) option
* with strike price X,
* where the underlying spot (share) price is S
*
* Trading of the option is assumed to start at some time
* t = 0, with expiration at some time T > 0,
* with time measured in units of years, or as a year fraction
*
* The option can be valued at any time t where
* 0 <= t <= T, remaining time to expiration (T-t)
*
* V = φ[S * e^(-q(T-t)) * N(φ*d1) - X * e^(-r(T-t)) *N(φ*d2)]
*
*   s = Spot price
*   X = strike price
*   φ = 1 for a call option, –1 for a put option
*   q = the continuous annual dividend rate
*   r = the continuous annual risk-free interest rate
*   σ = the annual volatility
*   N(x) = cumulative distribution function (CDF): (1+ errFx(x/sqrt_2))/2
*   d1 =  [log(S/X) + (r−q+ 1/2*σ^2) (T −t)]/[σ*sqrt(T−t)]
*   d2 = d1 - [σ*sqrt(T−t)]
*
*
* Implied volatility of an option
*   Given:  market price
*   Secant method => find the root of the function
*
*   f = V(σ; φ, S, r, q, t, T) −Vm
*
*   Vm = observed market price
*   V(..;..) = parameters on the right are fixed while left side varies
*/


// φ = pay off type
enum class PayOffType {
    call = 1,
    put = -1
};


class BlackScholes {
    double strikePrice,
        spotPrice,
        expiryTime,
        dividend,
        interestRate;

    PayOffType payOffType;
    std::array<double, 2>computeNormArgs(double volatility) const;  // function to calculate d1 & d2

public:
    BlackScholes(double strike_price, double spot_price, double expiry_time,
        double rate, PayOffType pay_off):
        strikePrice(strike_price),
        spotPrice(spot_price),
        expiryTime(expiry_time),
        dividend(0.0),
        interestRate(rate),
        payOffType(pay_off) {}

    double operator()(double vol);
    static double impliedVolatility(BlackScholes &bsc, double marketPrice,
        double x0, double x1, double total, unsigned maxIteration);

};


#endif //BLACKSCHOLES_H
