//
// Created by _edd.ie_ on 12/05/2025.
//

#include "Payoff.h"

#include <algorithm>


double CallPayoff::payoff(double price) const {
    return std::max(price - strike, 0.0);
}

CallPayoff *CallPayoff::clone() const {
    return new CallPayoff(*this);
}



double PutPayoff::payoff(double price) const {
    return std::max(price - strike, 0.0);
}

PutPayoff *PutPayoff::clone() const {
    return new PutPayoff(*this);
}

