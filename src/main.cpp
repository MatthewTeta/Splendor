/**
 * Author: Matthew Teta
 * Date: 11/25/2023
 *
 * File: main.cpp
 */

#include <iostream>

#include "Pieces.hpp"
#include "Resources.hpp"

int main() {
    Splendor::Card *c = new Splendor::Card(Splendor::Card::Type::ONE, 1, Splendor::Gemstone::Diamond, {{Splendor::Gemstone::Diamond, 1}});
    Splendor::Noble *n = new Splendor::Noble({{Splendor::Gemstone::Diamond, 1}});
    std::cout << c->get_points() << std::endl;


    

    return 0;



}

