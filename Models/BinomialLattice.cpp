//
// Created by _edd.ie_ on 13/05/2025.
//

#include "BinomialLattice.h"


BinomialLattice::BinomialLattice(const OptionInfo& opt, double vol, double intRate,
    unsigned timeSteps, double divRate) :   opt{std::move(opt)}, timeSteps{timeSteps + 1}, divRate{divRate}{
    double dt{opt.timeToExpiry() / timeSteps};
    u = std::exp(vol * std::sqrt(dt));
    d = 1.0 / u;
    p = 0.5 * (1.0 + (intRate - divRate - 0.5 * vol * vol) * std::sqrt(dt) / vol);
    discountFctr = std::exp(-intRate*dt);
    grid.resize(boost::extents[timeSteps][timeSteps]);
}

double BinomialLattice::calculatePrice(double spot, OptType optType) {
    project_underlying_prices_(spot);
    return calculate_node_payoffs_(optType); // grid[0][0].payoff
}


void BinomialLattice::project_underlying_prices_(double spot){
    grid[0][0].underlying = spot;        // Terminal node
    // j: columns, i: rows.
    // Traverse by columns, then set node in each row.
    for (int j = 1; j < timeSteps; ++j)
    {
        for (int i = 0; i <= j; ++i){
            if (i < j)
                grid[i][j].underlying = u * grid[i][j - 1].underlying;
            else  // (i == j)
                grid[i][j].underlying = d * grid[i - 1][j - 1].underlying;
        }
    }
}

double BinomialLattice::disc_expected_val_(int i, int j) const{
    return discountFctr * (p * grid[i][j + 1].payoff + (1.0 - p) * grid[i + 1][j + 1].payoff);
}

double BinomialLattice::calculate_node_payoffs_(OptType opt_type) {
    // Set the terminal nodes with payoffs at expiration: j = time_points_ - 1
    for (int i = 0; i <= timeSteps - 1; ++i)
    {
        grid[i][timeSteps - 1].payoff =
            opt.optionPayoff(grid[i][timeSteps - 1].underlying);
    }
    if (opt_type == OptType::American)
        american_payoffs_();
    else
        european_payoffs_();    // OptType::Euro
    return grid[0][0].payoff;    // Option value
}

void BinomialLattice::american_payoffs_(){
    // Start from the penultimate column prior to expiration: j = time_points_ - 2
    for (int j = timeSteps - 2; j >= 0; --j) {
        for (int i = 0; i <= j; ++i)
        {
            grid[i][j].payoff = std::max(disc_expected_val_(i, j), 
                opt.optionPayoff(grid[i][j].underlying));
        }
    }
}

void BinomialLattice::european_payoffs_(){
    // Start from the penultimate column prior to expiration: j = time_points_ - 2
    for (int j = timeSteps - 2; j >= 0; --j)
    {
        for (int i = 0; i <= j; ++i)
        {
            grid[i][j].payoff = disc_expected_val_(i, j);
        }
    }
}