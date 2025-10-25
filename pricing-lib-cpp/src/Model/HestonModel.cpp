#include "HestonModel.hpp"
#include <iostream>
using namespace std;

HestonModel::HestonModel() : Model()
{
}

HestonModel::HestonModel(const nlohmann::json json) : Model(json)
{
    json.at("correlation").get_to(rho_);
    json.at("k_").get_to(k_);
    json.at("theat").get_to(theta_);
    json.at("v_0").get_to(v_0);
    pathVect = pnl_vect_create_from_zero(N + 1);
    volVect = pnl_vect_create_from_zero(N + 1);
    W1 = pnl_vect_create_from_zero(N + 1);
    W2 = pnl_vect_create_from_zero(N + 1);
}

HestonModel::~HestonModel()
{
    pnl_vect_free(&pathVect);
    pnl_vect_free(&volVect);
    pnl_vect_free(&W1);
    pnl_vect_free(&W2);
}

void HestonModel::print() const
{
    std::cout << "====== Heston Model Characteristics ====" << std::endl;
    std::cout << " k_: " << k_ << std::endl;
    std::cout << " rho_: " << rho_ << std::endl;
    std::cout << " theta_: " << theta_ << std::endl;
    std::cout << " v_0 : " << v_0 << std::endl;

    Model::print();
}

void HestonModel::path(PnlRng *rng)
{
    simulation_W1_W2(W1, W2, rng);
    LET(volVect, 0) = v_0;
    LET(pathVect, 0) = GET(spot, 0);

    for (size_t i = 1; i <= N; i++)
    {
        // V[k] = V[k-1] + k_v*(theta - V[k-1])*(T/N) + sigma*np.sqrt(max(V[k-1] , 0))*(W_2[k] - W_2[k-1])
        LET(volVect, i) = GET(volVect, i - 1) + k_ * (theta_ - GET(volVect, i - 1)) * dt + GET(sigma_, 0) * sqrt(max(GET(volVect, i - 1), 0.0)) * (GET(W2, i) - GET(W2, i - 1));
        //  S[k] = S[k-1] + r*S[k-1]*(T/N) + np.sqrt(max(V[k-1] , 0))*S[k-1]*(W_1[k] - W_1[k-1])
        LET(pathVect, i) = GET(pathVect, i - 1) + r_ * GET(pathVect, i - 1) * dt + sqrt(max(GET(volVect, i - 1), 0.0)) * GET(pathVect, i - 1) * (GET(W1, i) - GET(W1, i - 1));
    }
}

void HestonModel::path(const PnlMat *past, double t, PnlRng *rng)
{
}

void HestonModel::simulation_W1_W2(PnlVect *W1, PnlVect *W2, PnlRng *rng)
{
    PnlVect *G = pnl_vect_create_from_zero(N);
    PnlVect *W3 = pnl_vect_create_from_zero(N + 1);

    // W1
    pnl_vect_rng_normal(G, N, rng);
    pnl_vect_mult_scalar(G, sqrt_dt);
    pnl_vect_cumsum(G);
    LET(W1, 0) = 0.0;
    for (size_t i = 0; i < N; i++)
    {
        LET(W1, i) = GET(G, i);
    }

    // W3 :
    pnl_vect_rng_normal(G, N, rng);
    pnl_vect_mult_scalar(G, sqrt_dt);
    pnl_vect_cumsum(G);
    LET(W3, 0) = 0.0;
    for (size_t i = 0; i < N; i++)
    {
        LET(W3, i) = GET(G, i);
    }

    // W2 :
    for (size_t i = 0; i < N; i++)
    {
        LET(W2, i) = rho_ * GET(W1, i) + std::sqrt(1 - rho_ * rho_) * GET(W3, i);
    }

    pnl_vect_free(&G);
    pnl_vect_free(&W3);
}
