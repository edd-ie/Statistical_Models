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
    std::unique_ptr<Payoff> payoff;
    double expirationTime;

public:
    OptionInfo(std::unique_ptr<Payoff> payoff, double expTime):payoff(std::move(payoff)), expirationTime(expTime){}
    double optionPayoff(double spot) const;
    double timeToExpiry() const;

    // Copy operations:
    OptionInfo(const OptionInfo& src): payoff(src.payoff->clone()), expirationTime(src.timeToExpiry()){}
    OptionInfo& operator =(const OptionInfo& src) noexcept;

    // Move operations:
    OptionInfo(OptionInfo&& rhs) = default;
    OptionInfo& operator =(OptionInfo&& src) = default;

    //swap function
    void swap(OptionInfo& src) noexcept;

    ~OptionInfo() = default;
};


#endif //OPTIONINFO_H
