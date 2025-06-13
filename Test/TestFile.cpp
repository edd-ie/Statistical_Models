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
    BlackScholes bsc_impl_vol{strike, spot, time_to_exp, rate, payoff_type, dividend};

    double impl_vol = BlackScholes::impliedVolatility(bsc_impl_vol, mkt_opt_price,
       init_vol_guess_1, init_vol_guess_2 , tol, max_iter);


    if (!std::isnan(impl_vol))
    {
        cout << format("Call ITM, time to expiration = {}, ", time_to_exp);
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


void testDate() {
    Act365 act_365{};
    Act360 act_360{};
    Thirty360 thirty_360{};

    Date sd_01{2021, 4, 26};
    Date ed_01{2023, 10, 26};

    Date sd_02{2022, 10, 10};
    Date ed_02{2023, 4, 10};

    // --- Act/365 Day Count Convention ---
    std::cout << "\n--- Act/365 Day Count ---\n";
    double yf_act_365_01 = act_365.year_fraction(sd_01, ed_01); // 2.50137
    std::cout << std::format("From {}-{:02d}-{:02d} to {}-{:02d}-{:02d} (Act/365): {:.5f} (Expected: 2.50137)\n",
            sd_01.year(), sd_01.month(), sd_01.day(), // <--- CALLING THE GETTERS
            ed_01.year(), ed_01.month(), ed_01.day(), // <--- CALLING THE GETTERS
            yf_act_365_01);

    double yf_act_365_02 = act_365.year_fraction(sd_02, ed_02); // 0.49863
    std::cout << std::format("From {}-{:02d}-{:02d} to {}-{:02d}-{:02d} (Act/365): {:.5f} (Expected: 0.49863)\n",
            sd_02.year(), sd_02.month(), sd_02.day(), // <--- CALLING THE GETTERS
            ed_02.year(), ed_02.month(), ed_02.day(), // <--- CALLING THE GETTERS
            yf_act_365_02);

    // --- Act/360 Day Count Convention ---
    std::cout << "\n--- Act/360 Day Count ---\n";
    double yf_act_360_01 = act_360.year_fraction(sd_01, ed_01); // 2.53611
    std::cout << std::format("From {}-{:02d}-{:02d} to {}-{:02d}-{:02d} (Act/360): {:.5f} (Expected: 2.53611)\n",
            sd_01.year(), sd_01.month(), sd_01.day(),
            ed_01.year(), ed_01.month(), ed_01.day(), yf_act_360_01);

    double yf_act_360_02 = act_360.year_fraction(sd_02, ed_02); // 0.505556
    std::cout << std::format("From {}-{:02d}-{:02d} to {}-{:02d}-{:02d} (Act/360): {:.6f} (Expected: 0.505556)\n",
            sd_02.year(), sd_02.month(), sd_02.day(),
            ed_02.year(), ed_02.month(), ed_02.day(), yf_act_360_02);

    // --- 30/360 Day Count Convention ---
    std::cout << "\n--- 30/360 Day Count ---\n";
    double yf_thirty_01 = thirty_360.year_fraction(sd_01, ed_01); // 2.5
    std::cout << std::format("From {}-{:02d}-{:02d} to {}-{:02d}-{:02d} (30/360): {:.1f} (Expected: 2.5)\n",
            sd_01.year(), sd_01.month(), sd_01.day(),
            ed_01.year(), ed_01.month(), ed_01.day(), yf_thirty_01);

    double yf_thirty_02 = thirty_360.year_fraction(sd_02, ed_02); // 0.5
    std::cout << std::format("From {}-{:02d}-{:02d} to {}-{:02d}-{:02d} (30/360): {:.1f} (Expected: 0.5)\n",
            sd_02.year(), sd_02.month(), sd_02.day(),
            ed_02.year(), ed_02.month(), ed_02.day(), yf_thirty_02);

    std::cout << "\n";
}


void testBond() {
    std::string bond_id = "20 yr bond";             // 20-year bond
    Date dated_date{2023, 5, 8};              // (Mon)
    Date first_coupon_date{2023, 11, 7};      // Short first coupon (Tue)
    Date penultimate_coupon_date{2042, 5, 7}; // (Wed)
    Date maturity_date{2043, 5, 7};   // Regular last coupon period (Thu)
    int coupon_frequency = 2;
    double coupon_rate = 0.062;
    double face_value = 1000.00;
    
    // Construction of the bond is then straightforward:
    Bond bond_20_yr{bond_id, dated_date, first_coupon_date, penultimate_coupon_date,
        maturity_date, coupon_frequency, coupon_rate, face_value};

    std::vector<Date> unit_bond_maturity_dates{{2023, 10, 11}, {2024, 1, 10},{2024, 4, 10},
        {2024, 10, 10}, {2025, 10, 10}, {2026, 10, 12}, {2028, 10, 10}, {2030, 10, 10},
        {2033, 10, 10}, {2038, 10, 11}, {2043, 10, 12}, {2053, 10, 10}};

    std::vector<double> unit_bond_prices{0.999945, 0.994489, 0.988210,
        0.973601, 0.939372, 0.901885, 0.827719, 0.759504,
        0.670094, 0.547598, 0.448541, 0.300886    };

    Date yc_settle_date{2023, 10, 10};
    Date bond_settle_date = yc_settle_date;

    LinearInterpYieldCurve yc{yc_settle_date, unit_bond_maturity_dates, unit_bond_prices};
    double value = bond_20_yr.discounted_value(bond_settle_date, yc);

    std::cout<< std::format("Bond payment after 20yrs : ${}", value);
}