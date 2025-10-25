#ifndef __HESTON_MODEL_HPP__
#define __HESTON_MODEL_HPP__

#include "Model.hpp"
#include "../Utils/json_helper.hpp"

class HestonModel : public Model
{
public:
    double rho_;       /// correlation of model
    double k_;         /// constant k of model
    double theta_;     /// constant theta of model
    double v_0;        /// volatility inital of market
    PnlVect *pathVect; /// vect to store the simulation of model
    PnlVect *volVect;  /// vect to store the volatility of market
    PnlVect *W1;       /// brownien 1 for asset
    PnlVect *W2;       /// brownien 2 for volatility

    HestonModel();
    HestonModel(const nlohmann::json json);
    ~HestonModel();
    void print() const override;
    void path(PnlRng *rng) override;
    void path(const PnlMat *past, double t, PnlRng *rng) override;

protected:
    void simulation_W1_W2(PnlVect *W1, PnlVect *W2, PnlRng *rng);
};

#endif