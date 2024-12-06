#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include "../Utils/convert.hpp"
#include "../Option/option.hpp"
#include "../monte_carlo/monte_carlo.hpp"

bool is_equals_array(double *ptr_arr_res, double *arr_expected, int size);

TEST(ConvertTest, TestForDataAsianJSON)
{

    MonteCarlo *monte_carlo = convert_json_to_monte_carlo("../../data/asian/asian.json");

    assert(monte_carlo->option->option_size == 2);
    EXPECT_TRUE(monte_carlo->option->strike == 100.0);

    double spots_expected[2] = {100.0, 100.0};
    EXPECT_TRUE(is_equals_array(monte_carlo->model->spots->array, spots_expected, 2) == true);

    EXPECT_TRUE(monte_carlo->option->maturity == 1.5);

    double vols_expected[2] = {0.2, 0.2};

    EXPECT_TRUE(is_equals_array(monte_carlo->model->volatility->array, vols_expected, 2) == true);

    EXPECT_TRUE(monte_carlo->model->interest_rate == 0.02);
    EXPECT_TRUE(monte_carlo->model->correlation == 0.0);

    EXPECT_TRUE(monte_carlo->option->type == OptionType::Asian);
    double payoff_coff_expected[2] = {0.5, 0.5};
    EXPECT_TRUE(is_equals_array(monte_carlo->option->payoff_coeffcients->array, payoff_coff_expected, 2) == true);

    EXPECT_TRUE(monte_carlo->fixing_dates_number == 24);
    assert(monte_carlo->sample_number == 50000);

    delete monte_carlo;
}

bool is_equals_array(double *ptr_arr_res, double *arr_expected, int size)
{
    // Comparaison manuelle élément par élément
    for (int i = 0; i < size; ++i)
    {
        if (ptr_arr_res[i] != arr_expected[i])
        {
            return false;
        }
    }
    return true;
}