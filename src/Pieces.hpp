/**
 * Author: Matthew Teta
 * Date: 11/25/2023
 *
 * File: Pieces.hpp
 */

#ifndef PIECES_HPP
#define PIECES_HPP

#include <string>
#include <vector>

namespace Splendor {

struct V2 {
    int row, col;
};

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
    static Type type_from_str(std::string type_str) {
        if (type_str == "Diamond")
            return Token::Type::Diamond;
        else if (type_str == "Sapphire")
            return Token::Type::Sapphire;
        else if (type_str == "Emerald")
            return Token::Type::Emerald;
        else if (type_str == "Ruby")
            return Token::Type::Ruby;
        else if (type_str == "Onyx")
            return Token::Type::Onyx;
        // else if (type_str == "Gold")
            return Token::Type::Gold;
    }

    // Constructor
    Token() = default;
    Token(Type a_token_type)
    : m_token_type(a_token_type) {};
    // No copy...
    // Token(const Token& o) = delete;
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
    static Type type_from_str(std::string level_str) {
        if (level_str == "ONE")
            return Card::Type::ONE;
        else if (level_str == "TWO")
            return Card::Type::TWO;
        // else if (level_str == "THREE")
            return Card::Type::THREE;
    }

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
