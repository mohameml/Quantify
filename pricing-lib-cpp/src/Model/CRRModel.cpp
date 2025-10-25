#include "CRRModel.hpp"
#include <iostream>
#include "../Utils/compute_last_index.hpp"

CRRModel::CRRModel() : Model()
{
}

CRRModel::CRRModel(const nlohmann::json json) : Model(json)
{
    a_ = (1 + r_) * std::exp(-GET(sigma_, 0) * sqrt_dt);
    b_ = (1 + r_) * std::exp(+GET(sigma_, 0) * sqrt_dt);
    p_ = (b_ - r_) / (a_ - r_);
    pathVect = pnl_vect_create_from_zero(N + 1);
}

CRRModel::~CRRModel()
{
    pnl_vect_free(&pathVect);
}

void CRRModel::print() const
{
    std::cout << "====== CRR Model Characteristics ====" << std::endl;
    std::cout << " a_: " << a_ << std::endl;
    std::cout << " b_: " << b_ << std::endl;
    std::cout << " p_: " << p_ << std::endl;
    Model::print();
}

void CRRModel::path(PnlRng *rng)
{
    double T_n = 0.0;
    LET(pathVect, 0) = GET(spot, 0);
    for (size_t i = 1; i <= N; i++)
    {
        T_n = pnl_rng_bernoulli(p_, rng) == 1 ? b_ : a_;
        LET(pathVect, i) = GET(pathVect, i - 1) * T_n;
    }
}

void CRRModel::path(const PnlMat *past, double t, PnlRng *rng)
{
    int last_i = compute_last_index(t, T_, N);
    PnlVect pastVect = pnl_vect_wrap_mat_row(past, 0);

    for (size_t j = 0; j <= last_i; j++)
    {
        LET(pathVect, j) = GET(&pastVect, j);
    }

    double T_n = 0.0;
    for (size_t i = last_i + 1; i <= N; i++)
    {
        T_n = pnl_rng_bernoulli(p_, rng) == 1 ? b_ : a_;
        LET(pathVect, i) = GET(pathVect, i - 1) * T_n;
    }
}
