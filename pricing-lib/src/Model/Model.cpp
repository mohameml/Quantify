#include "Model.hpp"
#include <iostream>

using namespace std;

/**
 *  the default constructeur
 */
Model::Model()
{
}

/**
 *
 * @param{in} json : object store the params of model
 */
Model::Model(const nlohmann::json json)
{
    json.at("interest rate").get_to(r_);
    json.at("maturity").get_to(T_);
    json.at("fixing dates number").get_to(N);
    json.at("option size").get_to(model_size);
    json.at("volatility").get_to(sigma_);
    if (sigma_->size == 1 && model_size > 1)
    {
        pnl_vect_resize_from_scalar(sigma_, model_size, GET(sigma_, 0));
    }
    json.at("spot").get_to(spot);
    if (spot->size == 1 && model_size > 1)
    {
        pnl_vect_resize_from_scalar(spot, model_size, GET(spot, 0));
    }
    dt = T_ / N;
    sqrt_dt = sqrt(dt);
}

/**
 * desctructeur
 */
Model::~Model()
{
    pnl_vect_free(&spot);
    pnl_vect_free(&sigma_);
}

/**
 * Print the model parameters
 */
void Model::print() const
{
    cout << " interest rate : " << r_ << endl;
    cout << " Spot : ";
    pnl_vect_print_asrow(spot);
    cout << "Maturity :" << T_ << endl;
    cout << " Number of TimeStepss : " << N << endl;
    cout << "sigma :" << GET(sigma_, 0) << endl;
    cout << "Model Size :" << model_size << endl;
}
