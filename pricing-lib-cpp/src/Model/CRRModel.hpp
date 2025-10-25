#ifndef __MODEL_CRR_HPP__
#define __MODEL_CRR_HPP_
#include "Model.hpp"
#include "../Utils/json_helper.hpp"

class CRRModel : public Model
{
public:
    double a_;         /// param a for CRR Model
    double b_;         /// param b for CRR Model
    double p_;         /// probablity risque-neutre in CRR Model
    PnlVect *pathVect; /// vect to store the simulation of model

    CRRModel();
    CRRModel(const nlohmann::json json);
    ~CRRModel();
    void print() const override;
    void path(PnlRng *rng) override;
    void path(const PnlMat *past, double t, PnlRng *rng) override;
};

#endif