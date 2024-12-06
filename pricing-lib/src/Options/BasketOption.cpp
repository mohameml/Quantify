#include "BasketOption.hpp"
#include <iostream>

BasketOption::BasketOption()
{
}

BasketOption::BasketOption(const nlohmann::json json) : Option(json)
{
    json.at("strike").get_to(strike);
    json.at("payoff coefficients").get_to(payoff_coeffcients);
    if (payoff_coeffcients->size == 1 && size > 1)
    {
        pnl_vect_resize_from_scalar(payoff_coeffcients, size, GET(payoff_coeffcients, 0));
    }
}

BasketOption::~BasketOption()
{
    pnl_vect_free(&this->payoff_coeffcients);
}

double BasketOption::payOff(const PnlMat *matrix)
{
    double res = 0;
    PnlVect ST = pnl_vect_wrap_mat_row(matrix, matrix->m - 1);
    res = pnl_vect_scalar_prod(&ST, payoff_coeffcients);
    return std::max(res - strike, 0.0);
}
