#ifndef OPTION_HPP
#define OPTION_HPP

#include <iostream>
#include "option_type.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "json_helper.hpp"

class Option
{
public:
    double maturity; /// maturité
    double dates;    /// nombre de dates d'exercice
    double size;     /// dimension du modèle, redondant avec BlackScholesModel::size

public:
    Option();
    /**
     * Constructeur de parsing :
     */
    Option(const nlohmann::json json);

    /**
     * Destructeur
     */
    virtual ~Option() {};

    /**
     * Calcule la valeur du payoff
     *
     * @param[in] matrix est une matrice  de taille (dates + 1)*size
     *  ligne d'index i de la matrice continet la valeur de l'actif du sous-jacent à t = t_i
     */
    virtual double payOff(const PnlMat *matrix) = 0;
};

/**
 * return la classe de l'option selon OptionType
 */
extern Option *instance_option(const nlohmann::json json);

#endif