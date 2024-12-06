#include <iostream>
#include "pnl/pnl_vector.h"
#include "MonteCarlo.hpp"
#include <fstream>
#include "pcpd_helper.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Le nombre d'arguments attendu est 2" << std::endl;
        exit(1);
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }
    nlohmann::json json = nlohmann::json::parse(file);
    MonteCarlo *monte_carlo = new MonteCarlo(json);

    double prix;
    double prix_std_dev;
    PnlVect *delta = pnl_vect_create_from_zero(monte_carlo->option->size);
    PnlVect *delta_std_dev = pnl_vect_create_from_zero(monte_carlo->option->size);

    monte_carlo->price_delta(prix, prix_std_dev, delta, delta_std_dev);

    PricingResults res(prix, prix_std_dev, delta, delta_std_dev);
    std::cout << res << std::endl;

    delete monte_carlo;
    pnl_vect_free(&delta);
    pnl_vect_free(&delta_std_dev);

    return 0;
}
