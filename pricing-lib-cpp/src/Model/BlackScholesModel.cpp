#include "BlackScholesModel.hpp"
#include <cmath>
#include <random>
#include "../Utils/compute_last_index.hpp"
#include <iostream>
#include <cassert>

BlackScholesModel::BlackScholesModel() : Model()
{
}

BlackScholesModel::BlackScholesModel(const nlohmann::json json) : Model(json)
{

    json.at("correlation").get_to(correlation);

    L = pnl_mat_create(model_size, model_size);
    pnl_mat_set_all(L, correlation);

    for (int i = 0; i < model_size; i++)
        pnl_mat_set_diag(L, 1.0, i);

    pnl_mat_chol(L);

    G = pnl_vect_create(model_size);

    pathMatrix = pnl_mat_create_from_zero(N + 1, model_size);
}

BlackScholesModel::~BlackScholesModel()
{

    pnl_vect_free(&G);
    pnl_mat_free(&L);
    pnl_mat_free(&pathMatrix);
}

/**
 * Génère une trajectoire du modèle et la stocke dans path
 * @param[out] pathMatrix contient une trajectoire du modèle.
 * C'est une matrice de taille (N+1) x D
 * @param[in] rng : génerateur des nombres aléatoires
 */

void BlackScholesModel::path(PnlRng *rng)
{
    int D = this->model_size;
    double r = r_;
    pnl_mat_set_row(pathMatrix, spot, 0);

    for (int i = 1; i < pathMatrix->m; i++)
    {
        pnl_vect_rng_normal(G, D, rng);
        for (int d = 0; d < D; d++)
        {
            double sigma_d = GET(sigma_, d);
            PnlVect L_d = pnl_vect_wrap_mat_row(L, d);
            MLET(pathMatrix, i, d) = MGET(pathMatrix, i - 1, d) * exp((r - sigma_d * sigma_d / 2.0) * (dt) + sigma_d * sqrt(dt) * pnl_vect_scalar_prod(&L_d, G));
        }
    }
}

/**
 * Génère une trajectoire du modèle et la stocke dans path (simulation conditionnelle)
 *
 * @param[out] pathMatrix contient une trajectoire du modèle.
 * C'est une matrice de taille (N+1) x D
 * @param[in] past : matrice de taille (i+1)*D avec i last-index
 * @param[in] t  :  temps actuel
 * @param[in] T  : maturité
 * @param[in] rng : génerateur des nombres aléatoires
 */

void BlackScholesModel::path(const PnlMat *past, double t, PnlRng *rng)
{
    int D = model_size;
    double r = r_;

    int last_index = compute_last_index(t, T_, pathMatrix->m - 1);

    if (last_index == pathMatrix->m - 1)
    {
        // pnl_mat_set_subblock(path, past, 0, 0);
        pnl_mat_extract_subblock(pathMatrix, past, 0, pathMatrix->m, 0, pathMatrix->n);
        return;
    }

    pnl_mat_set_subblock(pathMatrix, past, 0, 0);

    pnl_vect_rng_normal(G, D, rng);
    double dt_ = (last_index + 1) * dt - t;
    for (int d = 0; d < D; d++)
    {
        double s_t_d = MGET(past, past->m - 1, d);
        double sigma_d = GET(sigma_, d);
        PnlVect L_d = pnl_vect_wrap_mat_row(L, d);
        MLET(pathMatrix, last_index + 1, d) = s_t_d * exp((r - sigma_d * sigma_d / 2.0) * dt_ + sigma_d * sqrt(dt_) * pnl_vect_scalar_prod(&L_d, G));
    }

    for (int i = last_index + 2; i < pathMatrix->m; i++)
    {
        pnl_vect_rng_normal(G, D, rng);

        for (int d = 0; d < D; d++)
        {
            double s_t_d = MGET(pathMatrix, i - 1, d);
            double sigma_d = pnl_vect_get(sigma_, d);
            PnlVect L_d = pnl_vect_wrap_mat_row(L, d);
            MLET(pathMatrix, i, d) = s_t_d * exp((r - sigma_d * sigma_d / 2.0) * dt + sigma_d * sqrt(dt) * pnl_vect_scalar_prod(&L_d, G));
        }
    }
}

/**
 * shift path : original_paths[j, d] *= (1+h) j in {0 , ... , N}
 *
 * @param{in} d : the index of col to shift
 * @param{in} h : facteur to shift the col of matrix
 */

void BlackScholesModel::shift_asset(int d, double h)
{
    for (int i = 1; i < pathMatrix->m; i++)
    {
        // pnl_mat_set(original_paths, i, d, pnl_mat_get(original_paths, i, d) * h);
        MLET(pathMatrix, i, d) *= h;
    };
}

/**
 * shift path : original_paths[j, d] *= (1+h) j in {i , ... , N} , i = last_index(t)
 *
 * @param{in} d : the index of col to shift
 * @param{in} h : facteur to shift the col of matrix
 */

void BlackScholesModel::shift_asset(int d, double t, double h)
{
    int nb_lines = pathMatrix->m;
    int index = compute_last_index(t, T_, nb_lines - 1);
    for (int i = index + 1; i < nb_lines; i++)
    {
        // pnl_mat_set(original_paths, i, d, pnl_mat_get(original_paths, i, d) * h);
        MLET(pathMatrix, i, d) *= h;
    };
}

void BlackScholesModel::print() const
{
    std::cout << "====== BS Model Characteristics ====" << std::endl;
    std::cout << " correlation: " << correlation << std::endl;
    Model::print();
}
