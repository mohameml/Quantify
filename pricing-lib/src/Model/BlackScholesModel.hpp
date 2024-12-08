#ifndef BLACK_SCHOLES_MODEL_HPP
#define BLACK_SCHOLES_MODEL_HPP

#include "pnl/pnl_matvect.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "../Utils/json_helper.hpp"
#include "Model.hpp"

class BlackScholesModel : public Model
{
public:
    double correlation; /// paramètre de corrélation
    PnlMat *L;          /// raccine carrée de matrice de corrélation
    PnlVect *G;         /// Vector for simulation
    PnlMat *pathMatrix; /// matrix to store the simulation of model

public:
    BlackScholesModel();
    BlackScholesModel(const nlohmann::json json);
    ~BlackScholesModel();

    /**
     * Génère une trajectoire du modèle et la stocke dans path
     * @param[out] pathMatrix contient une trajectoire du modèle.
     * C'est une matrice de taille (N+1) x D
     * @param[in] rng : génerateur des nombres aléatoires
     */
    void path(PnlRng *rng) override;

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
    void path(const PnlMat *past, double t, PnlRng *rng) override;

    /**
     * simuler 2 trajectoires utilisant les mêmes aléas Browniens mais shiftées l’une par rapport à l’autre
     *
     * @param[out] original_path : multiplie la d-éme colone par (1+h)
     * @param[in] d : index of colonne
     * @param[in] h : double
     * @param[in] original_paths : matrice de taille (N + 1)*D qui contient la simulation du modéle
     */

    /**
     * shift path : original_paths[j, d] *= (1+h) j in {0 , ... , N}
     *
     * @param{in} d : the index of col to shift
     * @param{in} h : facteur to shift the col of matrix
     */
    void shift_asset(int d, double h);

    /**
     * shift path : original_paths[j, d] *= (1+h) j in {i , ... , N} , i = last_index(t)
     *
     * @param{in} d : the index of col to shift
     * @param{in} h : facteur to shift the col of matrix
     */
    void shift_asset(int d, double t, double h);

    void print() const override;
};
#endif