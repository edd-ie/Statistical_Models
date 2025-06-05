//
// Created by _edd.ie_ on 05/06/2025.
//

#include "EquityPriceGenerator.h"


/**
 * S_t = S_(t-1) * e^[(r-q-σ^2/2)Δt + σε_t*sqrt(Δt)]
 * S_t: stock price at t
 * S_(t-1): Previous stock price
 * r: risk-free rate
 * q: annualized dividend rate
 * σ: annualized volatility
 * ε_t: standard normal random variate at each t
 * S_0:  current spot price
 *
 * assumption: all the constructor arguments are greater than zero
 */

std::vector<double> EquityPriceGenerator::operator()(unsigned seed) const {
    std:: vector<double> sim;
    sim.reserve(timeSteps+1);

    std::mt19937_64 mt{seed};
    std::normal_distribution<>norm;

    auto stockPrice = [this](double prevPrice, double norm) {
        auto param1 = (rfRate-divRate - (volatility*volatility)/2)*deltaT;
        auto param2 = volatility*norm*sqrt(deltaT);

        return prevPrice * std::exp(param1+param2);
    };

    // Place the initial equity price into the 1st position in the vector:
    sim.push_back(spot);
    double equityPrice = spot;

    // Simulate underlying equity prices
    for (int i = 1; i<=timeSteps; i++) {
        equityPrice = stockPrice(equityPrice, norm(mt));
        sim.push_back(equityPrice);
    }

    return sim;
}