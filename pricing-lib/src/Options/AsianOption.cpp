#include "AsianOption.hpp"
#include <iostream>

AsianOption::AsianOption()
{
}

AsianOption::AsianOption(const nlohmann::json json) : Option(json)
{
    json.at("strike").get_to(strike);
    json.at("payoff coefficients").get_to(payoff_coeffcients);
    if (payoff_coeffcients->size == 1 && size > 1)
    {
        pnl_vect_resize_from_scalar(payoff_coeffcients, size, GET(payoff_coeffcients, 0));
    }
    vect_ones = pnl_vect_create_from_scalar(dates + 1, 1.0);
}

AsianOption::~AsianOption()
{
    pnl_vect_free(&this->payoff_coeffcients);
    pnl_vect_free(&vect_ones);
}

double AsianOption::payOff(const PnlMat *matrix)
{
    double res = pnl_mat_scalar_prod(matrix, vect_ones, payoff_coeffcients);
    res /= matrix->m;
    return std::max(res - strike, 0.0);
}
