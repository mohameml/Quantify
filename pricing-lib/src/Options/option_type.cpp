#include "option_type.hpp"
#include <iostream>

OptionType stringToOptionType(const std::string &type)
{
    if (type == "asian")
    {
        return OptionType::Asian;
    }
    else if (type == "basket")
    {
        return OptionType::Basket;
    }
    else if (type == "performance")
    {
        return OptionType::Performance;
    }
    else
    {
        throw std::invalid_argument("Type d'option non valide : " + type);
    }
}

std::string optionTypeToString(OptionType type)
{
    switch (type)
    {
    case OptionType::Basket:
        return "basket";
    case OptionType::Asian:
        return "asian";
    case OptionType::Performance:
        return "performance";
    default:
        return "Unknown";
    }
}
