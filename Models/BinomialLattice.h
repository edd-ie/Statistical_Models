//
// Created by _edd.ie_ on 13/05/2025.
//

#ifndef BINOMIALLATTICE_H
#define BINOMIALLATTICE_H


#include <boost/multi_array.hpp>
#include <algorithm>
#include <limits>
#include <utility>        // std::move
#include "../Options/OptionInfo.h"

/**
* Requires the payoff and time to expiration at each node
* based on a set of projected share prices.
*
* Other market data such as volatility, the dividend rate,
* and the risk-free interest rate, along with the desired
* number of time steps for the lattice,
*
* u = e^(Δt)^1/σ
* d = 1/u = e^-(Δt)^1/σ
* p = [e^(r-q)Δt - d]/[u - d]  ...probability
 */


enum class OptType {
    European,
    American
};



struct Node{
    double underlying;
    double payoff;
};


class BinomialLattice{
    OptionInfo opt;
    unsigned timeSteps;
    double divRate;

    double u{0.0}, d{0.0}, p{0.0}, discountFctr{0.0};
    boost::multi_array<Node, 2> grid;

    void project_underlying_prices_(double spot);
    double calculate_node_payoffs_(OptType opt_type);

    // Helper functions called from calculate_node_payoffs_(.):
    double disc_expected_val_(int i, int j) const;
    void american_payoffs_();
    void european_payoffs_();

public:
    BinomialLattice(const OptionInfo& opt, double vol, double intRate, unsigned timeSteps, double divRate=0.0);
    double calculatePrice(double spot, OptType optType);
};

#endif //BINOMIALLATTICE_H
