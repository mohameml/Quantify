#include <iostream>
#include "pnl/pnl_cdf.h"
#include <cmath>
#include <fstream>
#include "pnl/pnl_vector.h"
#include "../monte_carlo/monte_carlo.hpp"
#include "../Utils/convert.hpp"
#include "../Utils/utils.hpp"
#include <cmath>
#include <gtest/gtest.h>

void get_cotations(double t, PnlMat *past, PnlMat *market_data, MonteCarlo *monte_carlo)
{
    /*
    To use this function there are some steps to do :

    Step 1 :
    PnlVect* s_t = pnl_vect_new();
    PnlMat* cots = pnl_mat_new();

    get_cotations(t , cots , s_t);

    pnl_vect_free(&s_t);
    pnl_mat_free(&cots);
  */

    if (market_data == NULL)
    {
        throw std::invalid_argument("argument data.txt non fourni");
        exit(1);
    }

    int H = monte_carlo->hedging_dates_number;
    int N = monte_carlo->fixing_dates_number;
    double T = monte_carlo->option->maturity;
    int D = monte_carlo->option->option_size;

    int i = compute_last_index(t, T, N);

    pnl_mat_resize(past, i + 2, D);

    PnlVect *col = pnl_vect_create(D);

    for (int j = 0; j < i + 1; j++)
    {
        pnl_mat_get_row(col, market_data, j * H / N);
        pnl_mat_set_row(past, col, j);
    }

    // s_t
    pnl_mat_get_row(col, market_data, t * H / T);
    pnl_mat_set_row(past, col, i + 1);

    // free
    pnl_vect_free(&col);
}

void test(std::string name_file_json, std::string name_file_data)
{
    PnlMat *data = pnl_mat_create_from_file(name_file_data.c_str());
    MonteCarlo *monte_carlo = convert_json_to_monte_carlo(name_file_json);

    double H = monte_carlo->hedging_dates_number;
    int D = monte_carlo->option->option_size;
    PnlVect *vect_st = pnl_vect_create(D);
    PnlMat *past = pnl_mat_new();

    double t = 0.0;
    double v_t = 0.0;
    double v_t_std = 0.0;
    double v_t_expected = 0.0;
    double v_t_expected_std = 0.0;

    const double T_const = monte_carlo->option->maturity;
    PnlVect *spots_const = pnl_vect_create(D);
    pnl_vect_clone(spots_const, monte_carlo->model->spots);

    int pas = 20;
    for (int i = 1; i < H + 1; i += pas)
    {
        t = i * T_const / H;

        monte_carlo->option->maturity = T_const;
        pnl_vect_clone(monte_carlo->model->spots, spots_const);

        // calcul du v_t (pricer) :
        get_cotations(t, past, data, monte_carlo);
        monte_carlo->price(t, v_t, v_t_std, past);

        // calcul du v_t expected
        monte_carlo->option->maturity = T_const - t;
        pnl_mat_get_row(vect_st, past, past->m - 1);
        pnl_vect_clone(monte_carlo->model->spots, vect_st);
        monte_carlo->price(v_t_expected, v_t_expected_std);

        std::cout << "i  =" << i << std::endl;
        std::cout << "t  =" << t << std::endl;
        std::cout << "v_t  =" << v_t << std::endl;
        std::cout << "v_t_expected  =" << v_t_expected << std::endl;

        assert(std::fabs(v_t - v_t_expected) / v_t_expected < pow(10, -1));
    }

    std::cout << "\033[32m";
    std::cout << "PASSED" << std::endl;
    std::cout << "\033[0m";

    delete monte_carlo;
    pnl_mat_free(&data);
    pnl_mat_free(&past);
    pnl_vect_free(&vect_st);
    pnl_vect_free(&spots_const);
    
}

TEST(MonteCarloTest, TestingPriceCall)
{
    std::cout << "--------------------------- START Testing :  CALL ----------------------------------------------" << std::endl;
    test("../../data/call/call.json", "../../data/call/call_market.txt");
}

TEST(MonteCarloTest, TestingPriceBasket2D)
{
    std::cout << "--------------------------- START Testing :  Basket2D ----------------------------------------------" << std::endl;
    test("../../data/basket/basket_2d/basket_2d.json", "../../data/basket/basket_2d/basket_2d_market.txt");
}

TEST(MonteCarloTest, TestingPriceBasket5D)
{
    std::cout << "--------------------------- START Testing :  Basket5D ----------------------------------------------" << std::endl;
    test("../../data/basket/basket_5d/basket_5d.json", "../../data/basket/basket_5d/basket_5d_market.txt");
}

TEST(MonteCarloTest, TestingPriceBasket5D1)
{
    std::cout << "--------------------------- START Testing :  Basket5D1 ----------------------------------------------" << std::endl;
    test("../../data/basket/basket_5d_1/basket_5d_1.json", "../../data/basket/basket_5d_1/basket_5d_1_market.txt");
}

TEST(MonteCarloTest, TestingPriceBasket40D)
{
    std::cout << "--------------------------- START Testing :  Basket40D ----------------------------------------------" << std::endl;
    test("../../data/basket/basket_40d/basket_40d.json", "../../data/basket/basket_40d/basket_40d_market.txt");
}
