#include "PerformanceOption.hpp"

PerformanceOption::PerformanceOption()
{
}

PerformanceOption::PerformanceOption(const nlohmann::json json) : Option(json)
{
    json.at("payoff coefficients").get_to(payoff_coeffcients);
    if (payoff_coeffcients->size == 1 && size > 1)
    {
        pnl_vect_resize_from_scalar(payoff_coeffcients, size, GET(payoff_coeffcients, 0));
    }
}

PerformanceOption::~PerformanceOption()
{
    pnl_vect_free(&this->payoff_coeffcients);
}

double PerformanceOption::payOff(const PnlMat *matrix)
{
    double res = 0.0;

    for (int i = 1; i < matrix->m; i++)
    {
        PnlVect S_ti = pnl_vect_wrap_mat_row(matrix, i);
        PnlVect S_ti_1 = pnl_vect_wrap_mat_row(matrix, i - 1);
        double q = pnl_vect_scalar_prod(&S_ti, payoff_coeffcients) / pnl_vect_scalar_prod(&S_ti_1, payoff_coeffcients);
        res += std::max(q - 1.0, 0.0);
    }

    return res + 1;
}
