//
// Created by _edd.ie_ on 05/06/2025.
//
#include "TestFile.h"

// void testOptionInfo() {
//     auto call_payoff = std::make_unique<CallPayoff>(75.0);
//     OptionInfo call_opt{std::move(call_payoff), 0.5};
//
//     BinomialLattice binLattice{call_opt, 0.25, 0.04, 100, 0.02};
//     double optVal = binLattice.calculatePrice(85.0, OptType::American);
//
//     std::cout << format("Test Option info: {}\n", optVal);
// }



void testZmodel() {
    // TODO: T-model
    auto zModel = std::make_unique<ZScore>();
     // Forward lookup
     double zval = 2.0;
     double zdec = 0.05;
     double prob = zModel->getProbability(zval, zdec); // Gets probability for Z=-3.7, decimal=0.05

     // Gets Z-score for probability
     double alpha = 0.7881;
     double z = zModel->getScore(alpha);

     printf("P(X < %.2f) = %.4f\nZscore(%.4f) = %.2f\n", (zval+zdec), prob, alpha, z);
}


void testBlackScholes() {
    double strike = 75.0;
    auto payoff_type = PayOffType::call;
    double spot = 100.0;
    double rate = 0.05;
    double vol = 0.25;
    double time_to_exp = 0.0;

    // ITM Call at expiration (time_to_exp = 0):
    BlackScholes bsc_itm_exp{strike, spot, time_to_exp, rate, payoff_type};
    double value = bsc_itm_exp(vol);

    std::cout << std::format("ITM call with 0 dividend rate = ${}\n\n", value);


    strike = 95.0;
    double mkt_opt_price = 6.2;        // Current option price quoted in the market
    spot = 100.0;
    rate = 0.05;
    double dividend = 0.07;
    time_to_exp = 0.25;
    double init_vol_guess_1 = 0.1;
    double init_vol_guess_2 = 0.15;
    double tol = 1e-6;
    unsigned max_iter = 1000;
    BlackScholes bsc_impl_vol{strike, spot, time_to_exp, rate, payoff_type};

    double impl_vol = BlackScholes::impliedVolatility(bsc_impl_vol, mkt_opt_price,
       init_vol_guess_1, init_vol_guess_2 , tol, max_iter);


    if (!std::isnan(impl_vol))
    {
        cout << format("Call ITM, time to expiration = {}, ", time_to_exp);
        // Todo: fix implied volatility
        // expected value =  0.18501
        cout << format("Implied vol = {}\n", impl_vol);
        double opt_val = bsc_impl_vol(impl_vol);
        cout << format("Value of option at implied vol = {}, ", opt_val);
        cout << format("Market option price = {}\n", mkt_opt_price);
    }
    else
    {
        // Handle the case where the secant method fails to converge -
        // one option would be to throw an exception,
        // but more simply for this example:
        cout << "No convergence to implied volatility\n";
    }
}


void testMonteCarlo() {
    double strike = 75.0;
    double spot = 100.0;
    double vol = 0.25;
    double rate = 0.05;
    double div = 0.075;
    double time_to_exp = 0.5;
    unsigned num_time_steps = 12;
    unsigned num_scenarios = 20'000;
    int seed = 42;

    // --- First Monte Carlo Simulation (Non-Barrier) ---
    OptionInfo opt_call_itm_non_barrier{
        std::make_unique<CallPayoff>(strike), time_to_exp};

    MonteCarlo val_call_itm_non_barrier{std::move(opt_call_itm_non_barrier), // Move THIS OptionInfo
        num_time_steps, vol, rate, div};

    double opt_value = val_call_itm_non_barrier.calculatePrice(spot, seed, num_scenarios);

    printf("Computed option value in this case is $%.2f\n Expect : $23.51\n\n", opt_value);


    // --- Second Monte Carlo Simulation (Barrier) ---
    BarrierType barr_type = BarrierType::up_and_out;
    double barr_val = 110.0;

    OptionInfo opt_call_itm_barr{std::make_unique<CallPayoff>(strike), time_to_exp};

    MonteCarlo val_call_itm_barr{std::move(opt_call_itm_barr),
        num_time_steps, vol, rate, div, barr_val, barr_type};

    double opt_val = val_call_itm_barr.calculatePrice(spot, seed, num_scenarios);
    printf("Computed option value in this case is $%.2f\n Expect : $7.83\n\n", opt_val);

}