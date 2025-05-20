#include <iostream>
#include <memory>
#include <cmath>
#include <format>
#include <vector>

#include "Dataset.h"
#include "Models/BinomialLattice.h"
#include "Tables/ZScore.h"
#include "Models/BlackScholes.h"
#include "Options/OptionInfo.h"
#include "Options/Payoff.h"


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "curl/curl.h"
// #include <nlohmann/json.hpp>

void testBlackScholes();
void testOptionInfo();
void testZmodel();
// using json = nlohmann::json;

int main() {
    // testOptionInfo();

    CURL* curl = curl_easy_init();
    if (curl) {
        std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=AAPL&apikey=YOUR_API_KEY&datatype=json";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        if (CURLcode response = curl_easy_perform(curl); response != CURLE_OK) {
            std::cout << "Failed to fetch: " << curl_easy_strerror(response) << std::endl;
        } else {
            // Handle success
        }
    }


    curl_easy_cleanup(curl);

    return 0;
}

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
}