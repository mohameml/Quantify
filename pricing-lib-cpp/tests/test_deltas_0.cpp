#include <gtest/gtest.h>
#include "../Utils/convert.hpp"
#include "../monte_carlo/monte_carlo.hpp"
#include "../Utils/compute_last_index.hpp"
#include <iostream>
#include <fstream>
#include "../json_helper.hpp"
#include <cmath>

void is_equals_array(double *ptr_arr_res, double *arr_expected, int size)
{
    for (int i = 0; i < size; ++i)
    {
        std::cout << "calcul_t = " << ptr_arr_res[i] << std::endl;
        std::cout << "calcul_expected_t = " << arr_expected[i] << std::endl;
        assert(std::fabs(ptr_arr_res[i] - arr_expected[i]) / arr_expected[i] < pow(10, -1));
    }
}

// test deltas option call t=0
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

void test(std::string name_file_json, std::string name_file_json_expecetd)
{
    MonteCarlo *monte_carlo = convert_json_to_monte_carlo(name_file_json);

    int option_size = monte_carlo->option->option_size;
    PnlVect *deltas_vect = pnl_vect_create(option_size);
    PnlVect *stddev_deltas_vect = pnl_vect_create(option_size);
    monte_carlo->delta(deltas_vect, stddev_deltas_vect);

    PnlVect *deltas_vect_expecetd = pnl_vect_create(option_size);
    PnlVect *stddev_deltas_vect_expecetd = pnl_vect_create(option_size);
    std::ifstream file(name_file_json_expecetd);
    nlohmann::json json = nlohmann::json::parse(file);
    json.at("delta").get_to(deltas_vect_expecetd);
    json.at("deltaStdDev").get_to(stddev_deltas_vect_expecetd);

    is_equals_array(deltas_vect->array, deltas_vect_expecetd->array, option_size);
    is_equals_array(stddev_deltas_vect->array, stddev_deltas_vect_expecetd->array, option_size);

    std::cout << "\033[32m";
    std::cout << "PASSED" << std::endl;
    std::cout << "\033[0m";

    delete monte_carlo;
    pnl_vect_free(&deltas_vect);
    pnl_vect_free(&stddev_deltas_vect);
    pnl_vect_free(&deltas_vect_expecetd);
    pnl_vect_free(&stddev_deltas_vect_expecetd);
    file.close();
}

TEST(MonteCarloTest, TestingDeltasCall)
{
    std::cout << "============== Test CALL =======================" << std::endl;
    test("../../data/call/call.json", "../../data/call/call_expected_price.json");
}

TEST(MonteCarloTest, TestingDeltasAsian)
{
    std::cout << "============== Test Asian =======================" << std::endl;
    test("../../data/asian/asian.json", "../../data/asian/asian_expected_price.json");
}

TEST(MonteCarloTest, TestingDeltasBasket2D)
{
    std::cout << "============== Test Basket 2D =======================" << std::endl;
    test("../../data/basket/basket_2d/basket_2d.json", "../../data/basket/basket_2d/basket_2d_expected_price.json");
}

TEST(MonteCarloTest, TestingDeltasBasket5D)
{
    std::cout << "============== Test Basket 5D =======================" << std::endl;
    test("../../data/basket/basket_5d/basket_5d.json", "../../data/basket/basket_5d/basket_5d_expected_price.json");
}

TEST(MonteCarloTest, TestingDeltasBasket5D1)
{
    std::cout << "============== Test Basket 5D 1 =======================" << std::endl;
    test("../../data/basket/basket_5d_1/basket_5d_1.json", "../../data/basket/basket_5d_1/basket_5d_1_expected_price.json");
}

TEST(MonteCarloTest, TestingDeltasBasket40D)
{
    std::cout << "============== Test Basket 40D =======================" << std::endl;
    test("../../data/basket/basket_40d/basket_40d.json", "../../data/basket/basket_40d/basket_40d_expected_price.json");
}


TEST(MonteCarloTest, TestingDeltasPerf)
{
    std::cout << "============== Test Perf  =======================" << std::endl;
    test("../../data/perf/perf.json", "../../data/perf/perf_expected_price.json");
}
