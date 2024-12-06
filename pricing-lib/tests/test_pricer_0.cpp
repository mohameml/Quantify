#include <gtest/gtest.h>
#include "../Utils/convert.hpp"
#include "../monte_carlo/monte_carlo.hpp"
#include <iostream>
#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>

void test(std::string name_file_json, std::string name_file_json_expecetd)
{
    MonteCarlo *monte_carlo = convert_json_to_monte_carlo(name_file_json);
    double price = 0.0;
    double price_std_dev = 0.0;
    PnlVect *delta = pnl_vect_create(monte_carlo->option->option_size);
    PnlVect *delta_std_dev = pnl_vect_create(monte_carlo->option->option_size);
    monte_carlo->price(price, price_std_dev, delta, delta_std_dev);

    double price_expecetd;
    double price_std_dev_expecetd;
    std::ifstream file(name_file_json_expecetd);
    nlohmann::json json = nlohmann::json::parse(file);
    json.at("price").get_to(price_expecetd);
    json.at("priceStdDev").get_to(price_std_dev_expecetd);

    // std::cout << "price_t = " << price << std::endl;
    // std::cout << "expected_t = " << price_expecetd << std::endl;
    // std::cout << "price_std_dev_t = " << price_std_dev << std::endl;
    // std::cout << "price_std_dev_expected_t = " << price_std_dev_expecetd << std::endl;

    assert(std::fabs(price - price_expecetd) / price_expecetd < pow(10, -1));
    assert(std::fabs(price_std_dev - price_std_dev_expecetd) / price_std_dev_expecetd < pow(10, -1));
    std::cout << "\033[32m";
    std::cout << "PASSED" << std::endl;
    std::cout << "\033[0m";
    delete monte_carlo;
    pnl_vect_free(&delta);
    pnl_vect_free(&delta_std_dev);
    file.close();
}

TEST(MonteCarloTest, TestingPriceCall)
{
    std::cout << "--------------------------- START Testing :  CALL ----------------------------------------------" << std::endl;
    test("../../data/call/call.json", "../../data/call/call_expected_price.json");
}

TEST(MonteCarloTest, TestingPriceAsian)
{
    std::cout << "--------------------------- START Testing :  Asian ----------------------------------------------" << std::endl;
    test("../../data/asian/asian.json", "../../data/asian/asian_expected_price.json");
}

TEST(MonteCarloTest, TestingPriceBasket2D)
{
    std::cout << "--------------------------- START Testing :  Basket2D ----------------------------------------------" << std::endl;
    test("../../data/basket/basket_2d/basket_2d.json", "../../data/basket/basket_2d/basket_2d_expected_price.json");
}

TEST(MonteCarloTest, TestingPriceBasket5D)
{
    std::cout << "--------------------------- START Testing :  Basket5D ----------------------------------------------" << std::endl;
    test("../../data/basket/basket_5d/basket_5d.json", "../../data/basket/basket_5d/basket_5d_expected_price.json");
}

TEST(MonteCarloTest, TestingPriceBasket5D1)
{
    std::cout << "--------------------------- START Testing :  Basket5D1 ----------------------------------------------" << std::endl;
    test("../../data/basket/basket_5d_1/basket_5d_1.json", "../../data/basket/basket_5d_1/basket_5d_1_expected_price.json");
}

TEST(MonteCarloTest, TestingPriceBasket40D)
{
    std::cout << "--------------------------- START Testing :  Basket40D ----------------------------------------------" << std::endl;
    test("../../data/basket/basket_40d/basket_40d.json", "../../data/basket/basket_40d/basket_40d_expected_price.json");
}

TEST(MonteCarloTest, TestingPricePerf)
{
    std::cout << "--------------------------- START Testing :  Perf ----------------------------------------------" << std::endl;
    test("../../data/perf/perf.json", "../../data/perf/perf_expected_price.json");
}
