//
// Created by _edd.ie_ on 12/05/2025.
//

#ifndef OPTIONINFO_H
#define OPTIONINFO_H
#include "Payoff.h"

/**
* Hold the time to expiration in units of years (or a year fraction)
* Information about its payoff.
*
* The idea here is to hold information needed to value an option into a reusable class
* separate from a collection of pricing models that will require this information as input.
 */


class OptionInfo {
    double expirationTime;
    Payoff* payoff;

public:
    OptionInfo(Payoff* payoff, double expTime):payoff(payoff), expirationTime(expTime){}
    double optionPayoff(double spot) const;
    double timeToExpiry() const;
};


#endif //OPTIONINFO_H
