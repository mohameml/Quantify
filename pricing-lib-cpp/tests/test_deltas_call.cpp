#include <gtest/gtest.h>
#include "../Utils/convert.hpp"
#include "../monte_carlo/monte_carlo.hpp"
#include "../Utils/compute_last_index.hpp"
#include <iostream>
#include <fstream>
#include "../json_helper.hpp"
#include <cmath>
#include "pnl/pnl_cdf.h"


void deltaCall(PnlVect *St_i, double t, PnlVect *delta, MonteCarlo* mont)
{
    
    double inf = (GET(mont->model->volatility,0) * sqrt(mont->option->maturity - t));
    double d1 = (log(GET(St_i,0)/mont->option->strike) + (mont->model->interest_rate + pow(GET(mont->model->volatility,0),2)/2)*(mont->option->maturity - t))/inf;
    pnl_vect_set(delta,0,pnl_cdfnor(d1));
    
}


int main()
{
    MonteCarlo *monte_carlo = convert_json_to_monte_carlo("../../data/call/call.json");
    return 0;
}
