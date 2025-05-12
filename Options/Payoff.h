//
// Created by _edd.ie_ on 12/05/2025.
//

#ifndef PAYOFF_H
#define PAYOFF_H

/**
* call and put payoffs for equity options
* Derived from an abstract Payoff base class.
 */
class Payoff {
public:
    //return the payoff value for a given spot underlying price
    virtual double payoff(double price) const = 0; // pure virtual
    virtual Payoff* clone() const = 0;
    virtual ~Payoff() =default;
};


class CallPayoff final: public Payoff {
    double strike;
public:
    CallPayoff(double strike):strike(strike){}

    double payoff(double price) const override;
    CallPayoff* clone() const override;
};


class PutPayoff final: public Payoff {
    double strike;
public:
    PutPayoff(double strike):strike(strike){}

    double payoff(double price) const override;
    PutPayoff* clone() const override;
};




#endif //PAYOFF_H
