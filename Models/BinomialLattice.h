//
// Created by _edd.ie_ on 13/05/2025.
//

#ifndef BINOMIALLATTICE_H
#define BINOMIALLATTICE_H



#include "../Options/OptionInfo.h"

/**
* Requires the payoff and time to expiration at each node
* based on a set of projected share prices.
*
* Other market data such as volatility, the dividend rate,
* and the risk-free interest rate, along with the desired
* number of time steps for the lattice,
 */


enum class OptType {
    European,
    American
};


class BinomialLattice{
public:
    BinomialLattice(const OptionInfo& opt, double vol, double intRate,
        unsigned timeSteps, double divRate);
    double calculatePrice(double spot, OptType optType);
};

#endif //BINOMIALLATTICE_H
