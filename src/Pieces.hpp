/**
 * Author: Matthew Teta
 * Date: 11/25/2023
 *
 * File: Pieces.hpp
 */

#ifndef PIECES_HPP
#define PIECES_HPP

#include <vector>
#define NUM_MONETARY_RESOURCE_TYPES 5

namespace Splendor {

enum class Gemstone { Emerald, Sapphire, Ruby, Diamond, Onyx, Gold };


class Token {
    private:
        Gemstone m_token_type;
    
    public:
        Token(Gemstone a_token_type) : m_token_type(a_token_type) {};
};

class Card {
    using card_cost = std::array<int, NUM_MONETARY_RESOURCE_TYPES>;
    enum CardType { TIER_1, TIER_2, TIER_3 };
    
    private:
        CardType m_card_type;
        Gemstone m_token_type;
        card_cost m_card_cost;
  
    public:
        Card(CardType a_card_type) {
            switch(a_card_type) {
                case TIER_1:

                    pass;
                case TIER_2:
                    pass;
                case TIER_3:
                    pass;
            }
        } 
      

}

class Card : public Token {
public:
    enum Type {
        ONE,
        TWO,
        THREE,
    };

    // Constructor
    Card() = default;
    Card(Type a_card_type, Token::Type a_token_type, int a_points, std::vector<std::pair<int, Token::Type>> a_cost)
    : Token(a_token_type), m_card_type(a_card_type), m_points(a_points), m_cost(a_cost) {};
    // No copy...
    // Card(const Card& o) = delete;
    // Move is fine
    // Card (Card&& o)

    // Properties
    Type m_card_type;
    int m_points;
    std::vector<std::pair<int, Token::Type>> m_cost;

};

class Noble {
public:

    // Constructor
    Noble() = default;
    Noble(int a_points, std::vector<std::pair<int, Token::Type>> a_cost)
    : m_points(a_points), m_cost(a_cost) {};
    // No copy...
    // Noble(const Noble& o) = delete;
    // Move is fine
    // Noble (Noble&& o)

    // Properties
    int m_points;
    std::vector<std::pair<int, Token::Type>> m_cost;

};

}

#endif
