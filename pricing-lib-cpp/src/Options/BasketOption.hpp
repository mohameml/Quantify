#ifndef _Basket_Option_H
#define _Basket_Option_H

#include "Option.hpp"

class BasketOption : public Option
{

public:
    double strike;
    PnlVect *payoff_coeffcients;

    BasketOption();
    ~BasketOption();
    BasketOption(const nlohmann::json json);
    double payOff(const PnlMat *matrix) override;
};

#endif