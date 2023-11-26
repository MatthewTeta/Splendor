/**
 * Author: Matthew Teta
 * Date: 11/25/2023
 *
 * File: Board.hpp
 */

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
    std::array<std::optional<Noble>, 5> m_nobles;
    std::array<std::array<std::optional<Card>, 4>, 4> m_options;
    std::map<Token::Type, std::vector<Token>> m_bank;

    Board(
        std::array<Noble, 5> a_nobles,
        // std::array<Deck, 3> a_decks,
        std::array<Card, 40> a_deck_1,
        std::array<Card, 30> a_deck_2,
        std::array<Card, 20> a_deck_3,
    ) {
        // for (Noble& n : m_nobles) {
        //     n = std::make_optional(*a_nobles++);
        // }
        m_nobles = a_nobles;
    }
};

}

