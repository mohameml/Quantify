#include "BlackScholesModel.hpp"
#include <cmath>
#include <random>
#include "compute_last_index.hpp"
#include <iostream>
#include <cassert>

BlackScholesModel::BlackScholesModel()
{
}

BlackScholesModel::BlackScholesModel(const nlohmann::json json)
{
    double T;
    double N;
    json.at("maturity").get_to(T);
    json.at("fixing dates number").get_to(N);
    time_step = T / N;

    json.at("option size").get_to(model_size);
    json.at("interest rate").get_to(interest_rate);
    json.at("correlation").get_to(correlation);
    json.at("volatility").get_to(volatility);
    if (volatility->size == 1 && model_size > 1)
    {
        pnl_vect_resize_from_scalar(volatility, model_size, GET(volatility, 0));
    }
    json.at("spot").get_to(spots);
    if (spots->size == 1 && model_size > 1)
    {
        pnl_vect_resize_from_scalar(spots, model_size, GET(spots, 0));
    }

    L = pnl_mat_create(model_size, model_size);
    pnl_mat_set_all(L, correlation);

    for (int i = 0; i < model_size; i++)
        pnl_mat_set_diag(L, 1.0, i);

    pnl_mat_chol(L);

    G = pnl_vect_create(model_size);
}

BlackScholesModel::~BlackScholesModel()
{
    pnl_vect_free(&volatility);
    pnl_vect_free(&spots);
    pnl_vect_free(&G);
    pnl_mat_free(&L);
}

void BlackScholesModel::asset(PnlMat *path, PnlRng *rng)
{
    int D = this->model_size;
    double r = this->interest_rate;
    pnl_mat_set_row(path, spots, 0);

    for (int i = 1; i < path->m; i++)
    {
        pnl_vect_rng_normal(G, D, rng);
        for (int d = 0; d < D; d++)
        {
            double sigma_d = GET(volatility, d);
            PnlVect L_d = pnl_vect_wrap_mat_row(L, d);
            MLET(path, i, d) = MGET(path, i - 1, d) * exp((r - sigma_d * sigma_d / 2.0) * (time_step) + sigma_d * sqrt(time_step) * pnl_vect_scalar_prod(&L_d, G));
        }
    }
}

void BlackScholesModel::asset(const PnlMat *past, double t, double T, PnlMat *path, PnlRng *rng)
{
    int D = this->model_size;
    double r = this->interest_rate;

    int last_index = compute_last_index(t, T, path->m - 1);

    if (last_index == path->m - 1)
    {
        // pnl_mat_set_subblock(path, past, 0, 0);
        pnl_mat_extract_subblock(path, past, 0, path->m, 0, path->n);
        return;
    }

    pnl_mat_set_subblock(path, past, 0, 0);

    pnl_vect_rng_normal(G, D, rng);
    double dt = (last_index + 1) * time_step - t;
    for (int d = 0; d < D; d++)
    {
        double s_t_d = MGET(past, past->m - 1, d);
        double sigma_d = GET(volatility, d);
        PnlVect L_d = pnl_vect_wrap_mat_row(L, d);
        MLET(path, last_index + 1, d) = s_t_d * exp((r - sigma_d * sigma_d / 2.0) * dt + sigma_d * sqrt(dt) * pnl_vect_scalar_prod(&L_d, G));
    }

    for (int i = last_index + 2; i < path->m; i++)
    {
        pnl_vect_rng_normal(G, D, rng);

        for (int d = 0; d < D; d++)
        {
            double s_t_d = MGET(path, i - 1, d);
            double sigma_d = pnl_vect_get(this->volatility, d);
            PnlVect L_d = pnl_vect_wrap_mat_row(L, d);
            MLET(path, i, d) = s_t_d * exp((r - sigma_d * sigma_d / 2.0) * time_step + sigma_d * sqrt(time_step) * pnl_vect_scalar_prod(&L_d, G));
        }
    }
}

void BlackScholesModel::shift_asset(int d, double h, PnlMat *original_paths)
{
    for (int i = 1; i < original_paths->m; i++)
    {
        // pnl_mat_set(original_paths, i, d, pnl_mat_get(original_paths, i, d) * h);
        MLET(original_paths, i, d) *= h;
    };
}

void BlackScholesModel::shift_asset(int d, double t, double h, PnlMat *original_paths)
{
    int nb_lines = original_paths->m;
    double T = this->time_step * (nb_lines - 1);
    int index = compute_last_index(t, T, nb_lines - 1);
    for (int i = index + 1; i < nb_lines; i++)
    {
        // pnl_mat_set(original_paths, i, d, pnl_mat_get(original_paths, i, d) * h);
        MLET(original_paths, i, d) *= h;
    };
}
