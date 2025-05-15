//
// Created by _edd.ie_ on 12/05/2025.
//

#include "Payoff.h"

#include <algorithm>


double CallPayoff::payoff(double price) const {
    return std::max(price - strike, 0.0);
}

std::unique_ptr<Payoff> CallPayoff::clone() const {
    return std::make_unique<CallPayoff>(*this);
}



double PutPayoff::payoff(double price) const {
    return std::max(price - strike, 0.0);
}

std::unique_ptr<Payoff> PutPayoff::clone() const {
    return std::make_unique<PutPayoff>(*this);
}

