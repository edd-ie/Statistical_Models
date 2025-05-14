//
// Created by _edd.ie_ on 12/05/2025.
//

#ifndef OPTIONINFO_H
#define OPTIONINFO_H

#include "Payoff.h"
#include <utility>
#include <memory>

/**
* Hold the time to expiration in units of years (or a year fraction)
* Information about its payoff.
*
* The idea here is to hold information needed to value an option into a reusable class
* separate from a collection of pricing models that will require this information as input.
 */


class OptionInfo {
    double expirationTime;
    std::unique_ptr<Payoff> payoff;

public:
    OptionInfo(std::unique_ptr<Payoff> payoff, double expTime):expirationTime(expTime), payoff(std::move(payoff)){}
    double optionPayoff(double spot) const;
    double timeToExpiry() const;

    // Copy operations:
    OptionInfo(const OptionInfo& src):expirationTime(src.expirationTime), payoff(src.payoff->clone()){}
    OptionInfo& operator =(const OptionInfo src);

    // Disabling Move operations are discussed:
    OptionInfo(OptionInfo&& rhs) = delete;
    OptionInfo& operator =(OptionInfo&& rhs) = delete;

    //swap function
    void swap(OptionInfo& src) noexcept;

    ~OptionInfo();
};


#endif //OPTIONINFO_H
