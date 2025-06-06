//
// Created by _edd.ie_ on 15/05/2025.
//

#include "MonteCarlo.h"
#include "EquityPriceGenerator.h"

/**
 * Generating a single random price scenario.
 * To calculate the option price, iterate through a set of seeds to:
 * - Generate thousands of distinct and varying scenarios
 * - Calculate the terminal payoffs,
 * - Take the mean of the discounted payoffs.
*/
// e^[-r * 1/num_scenarios *Sum(x-strike)]
double MonteCarlo::calculatePrice(double spot, unsigned unifStartSeed, int numScenarios) {
    bool barrierHit = (barrierType == BarrierType::up_and_out && spot >= barrierValue)||
        (barrierType == BarrierType::down_and_out && spot <= barrierValue);

    if (barrierHit) return 0.0; // Option is worthless

    if (option.timeToExpiry() > 0.0) {
        using std::vector;

        std::mt19937_64 mtUnif{unifStartSeed};
        std::uniform_int_distribution<unsigned> unifDist{};

        vector<double> discountedPayoff;
        discountedPayoff.reserve(numScenarios);

        // e^[-r(T-t)]
        const double discFactor = std::exp(-intRate * option.timeToExpiry());

        for (int i=0; i<numScenarios; ++i) {
            EquityPriceGenerator gen{spot, vol, option.timeToExpiry(), intRate, divRate, timeSteps};

            // unifDist(mtUnif) provides the next seed):
            vector scenario = gen(unifDist(mtUnif));

            switch (barrierType) {
                case BarrierType::none: break;

                case BarrierType::up_and_out: {
                    auto barrierHitPos = std::find_if(scenario.cbegin(), scenario.cend(),
                        [this](double sim_eq) {return sim_eq >= barrierValue;});

                    if (barrierHitPos != scenario.cend())
                        barrierHit = true;
                } break;

                case BarrierType::down_and_out: {
                    auto barrierHitPos = std::ranges::find_if(scenario, [this](double sim_eq) {
                        return sim_eq <= barrierValue;
                    });

                    if (barrierHitPos != scenario.cend())
                        barrierHit = true;
                } break;
            }

            if (barrierHit)
                discountedPayoff.push_back(0.0);
            else
                discountedPayoff.push_back(discFactor * option.optionPayoff(scenario.back()));

            barrierHit = false;
        }

        // Option value = mean of discounted payoffs
        return (1.0/numScenarios) * std::accumulate(discountedPayoff.cbegin(), discountedPayoff.cend(), 0.0);
    }

    return option.optionPayoff(spot);  // barrier_hit == false, at expiration
}
