/**
 * Author: Matthew Teta
 * Date: 11/25/2023
 *
 * File: Board.hpp
 */

#ifndef BOARD_HPP
#define BOARD_HPP

#include <map>
#include <optional>
#include <array>
#include "Pieces.hpp"

namespace Splendor {

using Deck = std::map<Card::Type, std::vector<Card>>;

class Board {
private:
    Deck m_deck;
public:
    std::vector<Noble> m_nobles;
    std::array<std::vector<Card>, 3> m_options;
    std::map<Token::Type, std::vector<Token>> m_bank;

    Board(
        std::vector<Noble> &a_nobles,
        std::array<std::vector<Card>, 3> &a_options,
        std::map<Token::Type, std::vector<Token>> &a_bank
    ) {
        m_nobles = std::move(a_nobles);
        m_options = std::move(a_options);
        m_bank = std::move(a_bank);
    }
};

}

#endif