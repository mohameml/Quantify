#ifndef BLACK_SCHOLES_MODEL_HPP
#define BLACK_SCHOLES_MODEL_HPP
#include "pnl/pnl_matvect.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include "json_helper.hpp"

class BlackScholesModel
{
public:
    int model_size;       /// nombre d'actifs du modèle
    double interest_rate; /// taux d'intérêt
    double correlation;   /// paramètre de corrélation
    PnlVect *volatility;  /// vecteur de volatilités
    PnlVect *spots;       /// valeurs initiales des sous-jacents
    PnlMat *L;            /// raccine carrée de matrice de corrélation
    double time_step;     /// time step = T / N
    PnlVect *G;           /// Vector for simulation

public:
    BlackScholesModel();
    BlackScholesModel(const nlohmann::json json);
    ~BlackScholesModel();

    /**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (N+1) x D
     * @param[in] path : matrice de taille (N+1)xD
     * @param[in] rng : génerateur des nombres aléatoires
     */
    void asset(PnlMat *path, PnlRng *rng);

    /**
     * Génère une trajectoire du modèle et la stocke dans path (simulation conditionnelle)
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (N+1) x D
     * @param[in] past : matrice de taille (i+1)*D avec i last-index
     * @param[in] t  :  temps actuel
     * @param[in] T  : maturité
     * @param[in] path : matrice de taille (N+1)xD
     * @param[in] rng : génerateur des nombres aléatoires
     */
    void asset(const PnlMat *past, double t, double T, PnlMat *path, PnlRng *rng);

    /**
     * simuler 2 trajectoires utilisant les mêmes aléas Browniens mais shiftées l’une par rapport à l’autre
     *
     * @param[out] original_path : multiplie la d-éme colone par (1+h)
     * @param[in] d : index of colonne
     * @param[in] h : double
     * @param[in] original_paths : matrice de taille (N + 1)*D qui contient la simulation du modéle
     */
    void shift_asset(int d, double h, PnlMat *original_paths);
    void shift_asset(int d, double t, double h, PnlMat *original_paths);
};
#endif