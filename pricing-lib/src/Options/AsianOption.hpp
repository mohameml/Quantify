#ifndef _Asian_Optiont_H
#define _Asian_Option_H

#include "Option.hpp"

class AsianOption : public Option
{

public:
    double strike;
    PnlVect *payoff_coeffcients;
    PnlVect *vect_ones;
    AsianOption();
    AsianOption(const nlohmann::json json);
    ~AsianOption();

    double payOff(const PnlMat *matrix) override;
};

#endif