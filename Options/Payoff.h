//
// Created by _edd.ie_ on 12/05/2025.
//

#ifndef PAYOFF_H
#define PAYOFF_H
#include <memory>

/**
* call and put payoffs for equity options
* Derived from an abstract Payoff base class.
 */
class Payoff {
public:
    //return the payoff value for a given spot underlying price
    virtual double payoff(double price) const = 0; // pure virtual
    virtual std::unique_ptr<Payoff> clone() const = 0;
    virtual ~Payoff() =default;
};


class CallPayoff final: public Payoff {
    double strike;
public:
    explicit CallPayoff(double strike):strike(strike){}

    double payoff(double price) const override;
    std::unique_ptr<Payoff> clone() const override;
};


class PutPayoff final: public Payoff {
    double strike;
public:
    explicit PutPayoff(double strike):strike(strike){}

    double payoff(double price) const override;
    std::unique_ptr<Payoff> clone() const override;
};




#endif //PAYOFF_H
