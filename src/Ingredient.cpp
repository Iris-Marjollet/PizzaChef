#include <string>
#include "Ingredient.h"

using namespace std;

Ingredient::Ingredient(const std::string name)
{
    label = name;
}

std::string Ingredient::getlabel() const {
    return label;
}

