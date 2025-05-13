//
// Created by _edd.ie_ on 12/05/2025.
//

#include "OptionInfo.h"

double OptionInfo::optionPayoff(double spot) const {
    return payoff->payoff(spot);
}

double OptionInfo::timeToExpiry() const {
    return expirationTime;
}


OptionInfo& OptionInfo::operator=(OptionInfo src) {
    swap(src);
    return *this;
}

OptionInfo::~OptionInfo() {
    delete payoff;
}

void OptionInfo::swap(OptionInfo& src) noexcept {
    using std::swap;
    swap(payoff, src.payoff);
    swap(expirationTime, src.expirationTime);
}
