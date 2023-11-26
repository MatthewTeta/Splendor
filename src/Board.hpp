/**
 * Author: Matthew Teta
 * Date: 11/25/2023
 *
 * File: Board.hpp
 */

#ifndef BOARD_HPP
#define BOARD_HPP

#include <iterator>
#include <map>
#include "Pieces.hpp"

namespace Splendor {

using Deck = std::map<Card::Type, std::vector<Card>>;

class Board {
private:
    Deck m_deck;
public:
    std::vector<Noble> m_nobles;
    Deck m_options;
    std::map<Token::Type, std::vector<Token>> m_bank;

    Board() = default;
    Board(
        std::vector<Noble> &a_nobles,
        Deck &a_deck,
        std::map<Token::Type, std::vector<Token>> &a_bank
    ) {
        m_nobles = std::move(a_nobles);
        m_deck = std::move(a_deck);
        m_bank = std::move(a_bank);
    }

    void shuffle() {
        for (auto rows : m_deck) {
            // Put all cards back into the deck
            auto deck = rows.second;
            auto type = rows.first;
            auto layout = m_options[type];
            while (layout.size()) {
                // replenish from deck
                deck.insert(deck.end(),
                                    std::make_move_iterator(layout.end() - 1),
                                    std::make_move_iterator(layout.end()));
                layout.erase(layout.end() - 1);
            }

            // Randomize the decks
            for (int i = 0; i < deck.size(); i++) {
                int r = std::rand() % deck.size();
                std::swap(deck.at(i), deck.at(r));
            }
        }

        // Randomize the Nobles
        for (int i = 0; i < m_nobles.size(); i++) {
            int r = std::rand() % m_nobles.size();
            std::swap(m_nobles.at(i), m_nobles.at(r));
        }
    }

    void prepare_for_turn() {
        // Make sure to replenish the board if possible
        for (auto rows : m_options) {
            auto layout = rows.second;
            auto type = rows.first;
            auto deck = m_deck[type];
            while (layout.size() < 4 && deck.size()) {
                // replenish from deck
                layout.insert(layout.end(),
                                    std::make_move_iterator(deck.end() - 1),
                                    std::make_move_iterator(deck.end()));
                deck.erase(deck.end() - 1);
            }
        }
    }
};

}

#endif
