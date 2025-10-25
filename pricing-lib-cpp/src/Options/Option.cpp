#include "Option.hpp"

#include "string.h"
#include "BasketOption.hpp"
#include "AsianOption.hpp"
#include "PerformanceOption.hpp"
#include <iostream>

using namespace std;

Option::Option()
{
}

Option::Option(const nlohmann::json json)
{

    json.at("maturity").get_to(maturity);
    json.at("option size").get_to(size);
    json.at("fixing dates number").get_to(dates);
}

Option *instance_option(const nlohmann::json json)
{
    Option *opt = NULL;
    string optionType;
    json.at("option type").get_to(optionType);

    if (optionType == "basket")
        opt = new BasketOption(json);
    else if (optionType == "asian")
        opt = new AsianOption(json);
    else if (optionType == "performance")
        opt = new PerformanceOption(json);
    else
    {
        cout << "Option " << optionType << " unknow. Abort." << endl;
        abort();
    }

    return opt;
}
