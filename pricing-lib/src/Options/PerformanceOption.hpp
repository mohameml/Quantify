#ifndef _Performance_Option_H
#define _Performance_Option_H

#include "Option.hpp"

class PerformanceOption : public Option
{

public:
    PnlVect *payoff_coeffcients;

    PerformanceOption();
    ~PerformanceOption();
    PerformanceOption(const nlohmann::json json);
    double payOff(const PnlMat *matrix) override;
};

#endif