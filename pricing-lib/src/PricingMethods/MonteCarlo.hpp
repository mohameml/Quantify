#ifndef MONTE_CARLO_HPP
#define MONTE_CARLO_HPP

#include "Option.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_vector.h"

class MonteCarlo
{
public:
    Option *option;           /// pointeur sur l'option
    BlackScholesModel *model; /// pointeur vers le modèle
    int fixing_dates_number;  /// N Nombre de dates de discrétisation
    int sample_number;        /// nombre de tirage de MC
    double fd_step;           /// pas de méthode de différance fini
    int hedging_dates_number; /// Nombre de discrétisation  pour la couverture
    PnlRng *rng;              /// génrateur de nombre aléatoire

public:
    MonteCarlo();
    MonteCarlo(const nlohmann::json json);
    ~MonteCarlo();

    /**
     * calcul de la valeur du  prix de l'option à t=0 avec une méthode de MC classique
     *
     * @param[in] price : valeur finale du price
     * @param[in] price_std : valeur finale du ecart-type du price
     */
    void price(double &price, double &price_std);

    /**
     * calcul du prix à t = 0  avec le delta de l'option à t = 0  :
     *
     * @param[in] price : valeur finale du price
     * @param[in] price_std : valeur finale du ecart-type de price
     * @param[in] deltas_vect : vecteur de deltas
     * @param[in] stddev_deltas_vect : vecteur des ecart-types pour les deltas
     */
    void price_delta(double &price, double &price_std, PnlVect *deltas_vect, PnlVect *stddev_deltas_vect);

    /**
     * calcul du prix à t  avec le delta de l'option à t   :
     *
     * @param[in] t : current time
     * @param[in] price : valeur finale du price
     * @param[in] price_std : valeur finale du ecart-type de price
     * @param[in] Past : matrice de taille (last_index + 1)*D ou (last_index)*D qui continet s_t0 , s_t1 , ..... , st_i , st
     * @param[in] deltas_vect : vecteur de deltas
     * @param[in] stddev_deltas_vect : vecteur des ecart-types pour les deltas
     */
    void price_delta(double t, double &price, double &price_std, const PnlMat *Past, PnlVect *deltas_vect, PnlVect *stddev_deltas_vect);

    /**
     * Calcul P&L
     *
     * @param[in] market_data  : matrice de taille (H +1)*D qui contient les prix du sous-jacent  observés sur le marché aux instants ti = i*T/H
     * @param[in] p_and_l : valeur finale du P&L
     */
    void calculPAndL(PnlMat *market_data, double &p_and_l);

protected:
    /**
     * terime le  calucl de prix et std_prix :
     *
     * @param[in, out] price
     * @param[in, out] stdDev
     * @param nSamples : nombre de tirage de MC
     * @param t current date
     */
    void end_of_calcul_price(double &price, double &stdDev, double t) const;

    /**
     * terime le  calucl de delta :
     *
     * @param[in, out] price
     * @param[in, out] stdDev
     * @param nSamples : nombre de tirage de MC
     * @param t current date
     * @param St : vecteur de prix du sous-jacent
     */
    void end_of_calcul_delta(PnlVect *delta, PnlVect *delta_stdev, double t, PnlVect *St) const;
};

#endif