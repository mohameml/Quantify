#include <iostream>
#include "json_helper.hpp"
#include "pcpd_helper.hpp"

std::ostream&
operator<<(std::ostream& o, const HedgingResults& res)
{
    nlohmann::json j = {
        { "initialPrice", res.initialPrice },
        { "initialPriceStdDev", res.initialPriceStdDev },
        { "finalPnL", res.finalPnL }
    };
    o << std::setw(4) << j << std::endl;
    return o;
}

std::ostream&
operator<<(std::ostream& o, const PricingResults& res)
{

    nlohmann::json j = {
        { "price", res.price },
        { "priceStdDev", res.priceStdDev },
        { "delta", res.delta },
        { "deltaStdDev", res.deltaStdDev }
    };
    o << std::setw(4) << j << std::endl;
    return o;
}