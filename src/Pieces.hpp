/**
 * Author: Matthew Teta
 * Date: 11/25/2023
 *
 * File: Pieces.hpp
 */

#ifndef PIECES_HPP
#define PIECES_HPP

#include <iostream>
#include <vector>
#include <array>
#include <map>


#define NUM_MONETARY_RESOURCE_TYPES 5

namespace Splendor {

enum class Gemstone { Emerald, Sapphire, Ruby, Diamond, Onyx, Gold };

class Token {
    public:
        Token(Gemstone a_token_type) : m_token_type(a_token_type) {};
        // No copy constructor
        Token(const Token &a_token) = delete;

        // Getter methods
        const Gemstone get_token_type() const { return m_token_type; };

    private:
        Gemstone m_token_type;
    
};

class Card {
  
    public:
        using card_cost = std::map<Gemstone, int>;
        enum Type { ONE, TWO, THREE };
        
        Card(Type a_card_type, int a_points, Gemstone a_token_type, card_cost a_card_cost) : m_card_type(a_card_type), m_points(a_points), m_token_type(a_token_type), m_card_cost(a_card_cost) {};
        // No copy 
        Card(const Card &a_card) = delete;

        // Getter methods
        const Type get_card_type() const { return m_card_type; };
        const int get_points() const { return m_points; };
        const Gemstone get_token_type() const { return m_token_type; };
        const card_cost get_card_cost() const { return m_card_cost; };

    private:
        const Type m_card_type;
        const int m_points;
        const Gemstone m_token_type;
        const card_cost m_card_cost;
};

class Noble {
    
    public:
        using noble_cost = std::map<Gemstone, int>;                                                                     `
        static const int m_points = 3;
        Noble(noble_cost a_noble_cost) : m_noble_cost(a_noble_cost) {};
    private:
        const noble_cost m_noble_cost;
    
};

}

#endif