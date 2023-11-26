/**
 * Author: Matthew Teta
 * Date: 11/25/2023
 *
 * File: Pieces.hpp
 */

#include <iostream>
#include <vector>

namespace Splendor {

class Token {
public:
    enum Type {
        Emerald,
        Sapphire,
        Ruby,
        Diamond,
        Onyx,
        Gold,
    };

    // Constructor
    Token (Type a_token_type) {
        this->m_token_type = a_token_type;
    };
    // No copy...
    Token (const Token& o) = delete;
    // Move is fine
    // Token (Token&& o)

    // Properties
    Type m_token_type;

};

class Card : public Token {
public:
    enum Type {
        ONE,
        TWO,
        THREE,
    };

    // Constructor
    Card (Type a_card_type, Token::Type a_token_type, int a_points, std::vector<Token::Type> a_cost)
    : Token(a_token_type)
    {
        m_card_type = a_card_type;
        m_points = a_points;
        m_cost = a_cost;
    };
    // No copy...
    Card (const Card& o) = delete;
    // Move is fine
    // Card (Card&& o)

    // Properties
    Type m_card_type;
    int m_points;
    std::vector<Token::Type> m_cost;

};

class Noble {
public:
    // Constructor
    Noble (int a_points, std::vector<Token::Type> a_cost)
    {
        m_points = a_points;
        m_cost = a_cost;
    }
    // No copy...
    Noble (const Noble& o) = delete;
    // Move is fine
    // Noble (Noble&& o)

    // Properties
    int m_points;
    std::vector<Card::Type> m_cost;

};

class Player {
public:
    std::vector<Token> m_tokens;
    std::vector<Card>  m_cards;
    std::vector<Card>  m_reserve_cards_public;
private:
    std::vector<Card>  m_reserve_cards_private;

};

}

