#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include "pnl/pnl_vector.h"
#include "../Utils/json_helper.hpp"
#include "pnl/pnl_random.h"

class Model
{
public:
    double r_;       /// interstae rate of model r
    PnlVect *spot;   /// init value of underline asset :S_0
    double T_;       ///  maturity of the option : T
    int N;           ///  step for time discretisation : N
    PnlVect *sigma_; /// volatility of model*
    double dt;       /// time step = T / N
    double sqrt_dt;  /// sqrt(T/N)
    int model_size;  /// the size of model

    /**
     *  the default constructeur
     */
    Model();

    /**
     *
     * @param{in} json : object store the params of model
     */
    Model(const nlohmann::json json);

    /**
     * desctructeur
     */
    virtual ~Model();

    /**
     * Print the model parameters
     */
    virtual void print() const;

    /**
     * Compute one path of the model with and store it in pathMatrix
     *
     * @param{in} rng : random number generator
     */
    virtual void path(PnlRng *rng) = 0;

    /**
     *  Compute one conditional path and store it in pathMatrix
     *
     * @param{in} past : matrix to store the past value of underline asset
     * @param{in} t : current time
     * @param{in} rng :  random number generator
     */
    virtual void path(const PnlMat *past, double t, PnlRng *rng) = 0;
};

#endif