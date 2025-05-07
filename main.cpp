#include <iostream>
#include <memory>
#include <cmath>
#include <format>
#include <vector>

#include "Dataset.h"
#include "Tables/ZScore.h"
#include "Models/BlackScholes.h"



// Function to calculate the cumulative distribution function (CDF) for the standard normal distribution
double normalCDF(double value) {
    return 0.5 * erfc(-value * M_SQRT1_2);
}

// Function to generate a Z-table
std::vector<std::vector<double>> generateZTable() {
    std::vector<std::vector<double>> zTable(10, std::vector<double>(10));
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            double zValue = i * 0.1 + j * 0.01;
            zTable[i][j] = normalCDF(zValue);
        }
    }
    return zTable;
}

// Function to print the Z-table
void printZTable(const std::vector<std::vector<double>>& zTable) {
    std::cout << "Z-Table:\n";
    for (const auto& row : zTable) {
        for (double value : row) {
            std::cout << value << " ";
        }
        std::cout << "\n";
    }
}

int main() {

    // std::unique_ptr<Dataset> test = make_unique<Dataset>();
    //
    // test->importData("../resources/dummy2.csv");
    // test->printData();

    // auto zTable = generateZTable();
    // printZTable(zTable);

    //TODO: T-model
    //auto zModel = std::make_unique<ZScore>();
    // // Forward lookup
    // double zval = 2.0;
    // double zdec = 0.05;
    // double prob = zModel->getProbability(zval, zdec); // Gets probability for Z=-3.7, decimal=0.05
    //
    // // Gets Z-score for probability
    // double alpha = 0.7881;
    // double z = zModel->getScore(alpha);
    //
    // printf("P(X < %.2f) = %.4f\nZscore(%.4f) = %.2f\n", (zval+zdec), prob, alpha, z);

    double strike = 75.0;
    auto payoff_type = PayOffType::call;
    double spot = 100.0;
    double rate = 0.05;
    double vol = 0.25;
    double time_to_exp = 0.0;

    // ITM Call at expiration (time_to_exp = 0):
    BlackScholes bsc_itm_exp{strike, spot, time_to_exp, rate, payoff_type};
    double value = bsc_itm_exp(vol);

    std::cout << std::format("ITM call with 0 dividend rate = ${}\n", value);


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

    return 0;
}