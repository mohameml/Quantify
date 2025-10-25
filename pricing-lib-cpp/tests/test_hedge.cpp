#include <iostream>
#include "../monte_carlo/monte_carlo.hpp"
#include "../Utils/convert.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

void test(std::string name_file_json, std::string name_file_data, std::string name_file_json_expected)
{
    MonteCarlo *monte_carlo = convert_json_to_monte_carlo(name_file_json);
    PnlMat *data = pnl_mat_create_from_file(name_file_data.c_str());
    double pAndL;
    monte_carlo->calculPAndL(data, pAndL);

    double pAndL_expected;
    std::ifstream file(name_file_json_expected);
    nlohmann::json json = nlohmann::json::parse(file);
    json.at("finalPnL").get_to(pAndL_expected);

    std::cout << "pAndL = " << pAndL << std::endl;
    std::cout << "pAndL_expected = " << pAndL_expected << std::endl;

    assert(std::fabs(pAndL_expected - pAndL) / std::fabs(pAndL_expected) < pow(10, -1));
    std::cout << "\033[32m";
    std::cout << "PASSED" << std::endl;
    std::cout << "\033[0m";

    delete monte_carlo;
    pnl_mat_free(&data);
    file.close();
}

int main()
{
    // test("../../data/call/call.json", "../../data/call/call_market.txt", "../../data/call/call_expected_hedge.json");
    test("../../data/asian/asian.json", "../../data/asian/asian_market.txt", "../../data/asian/asian_expected_hedge.json");
    return 0;
}