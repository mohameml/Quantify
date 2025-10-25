#ifndef OPTION_TYPE_HPP
#define OPTION_TYPE_HPP
#include <string>
#include <iostream>

enum OptionType
{
    Basket,
    Asian,
    Performance
};

// Fonction pour convertir une chaîne en OptionType
OptionType stringToOptionType(const std::string &type);

// Fonction pour convertir OptionType en chaîne de caractères
std::string optionTypeToString(OptionType type);

#endif