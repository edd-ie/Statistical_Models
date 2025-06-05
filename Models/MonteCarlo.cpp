//
// Created by _edd.ie_ on 15/05/2025.
//

#include "MonteCarlo.h"
#include "EquityPriceGenerator.h"


// e^[-r * 1/num_scenarios *Sum(x-strike)]
double MonteCarlo::calculatePrice(double spot, unsigned unifStartSeed, int numScenarios) {
    if (option.timeToExpiry()>0.0) {
        using std::vector;

        std::mt19937_64 mtUnif{unifStartSeed};
        std::uniform_int_distribution<unsigned> unifDist{};

        vector<double> discountedPayoff;
        discountedPayoff.reserve(numScenarios);

        // e^[-r(T-t)]
        const double discFactor = std::exp(-intRate*option.timeToExpiry());

        for (int i=0; i<numScenarios; ++i) {
            EquityPriceGenerator gen{spot, vol, option.timeToExpiry(), intRate, divRate, timeSteps};

            // unifDist(mtUnif) provides the next seed):
            vector scenario = gen(unifDist(mtUnif));

            discountedPayoff.push_back(discFactor*option.optionPayoff(scenario.back()));
        }

        return (1.0/numScenarios) * std::accumulate(discountedPayoff.cbegin(), discountedPayoff.cend(), 0.0);
    }

    return option.optionPayoff(spot);
}
