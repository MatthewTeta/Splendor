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
#include <random>
#include <algorithm>
#include "Pieces.hpp"

namespace Splendor {

class Deck {
    using _Deck = std::map<Card::Type, std::vector<Card>>;
    _Deck _this;

public:
    Deck() {
        // Insert all card types
        // _this.insert(Card::Type::ONE, {});
        // _this.insert(Card::Type::TWO, {});
        // _this.insert(Card::Type::THREE, {});
        _this = {
            {
                Card::Type::ONE, {},
            },
            {
                Card::Type::TWO, {},
            },
            {
                Card::Type::THREE, {},
            },
        };
    }

    Deck& operator+=(Card& rhs) {
        _this.at(rhs.m_card_type).push_back(rhs);
        return *this;
    }

    std::vector<Card>& operator[](Card::Type a_type) {
        return _this.at(a_type);
    }

    _Deck& getMap() {
        return _this;
    }
};

class Bank {
    using _Bank = std::map<Token::Type, std::vector<Token>>;
    _Bank _this;

public:
    Bank() {
        // Insert all token types
        _this = {
            {
                Token::Type::Emerald,
                { 7, Token(Token::Type::Emerald) }
            },
            {
                Token::Type::Sapphire,
                { 7, Token(Token::Type::Sapphire) }
            },
            {
                Token::Type::Ruby,
                { 7, Token(Token::Type::Ruby) }
            },
            {
                Token::Type::Diamond,
                { 7, Token(Token::Type::Diamond) }
            },
            {
                Token::Type::Onyx,
                { 7, Token(Token::Type::Onyx) }
            },
            {
                Token::Type::Gold,
                { 5, Token(Token::Type::Gold) }
            },
        };
    }

    Bank& operator+=(Token& rhs) {
        _this.at(rhs.m_token_type).push_back(rhs);
        return *this;
    }

    std::vector<Token>& operator[](Token::Type a_type) {
        return _this.at(a_type);
    }

    _Bank& getMap() {
        return _this;
    }
};

class Board {
private:
    Deck m_deck;
public:
    std::vector<Noble> m_nobles;
    Deck m_options;
    Bank m_bank;

    Board() = default;
    Board(
        std::vector<Noble> &a_nobles,
        Deck &a_deck,
        Bank &a_bank
    ) {
        m_nobles = std::move(a_nobles);
        m_deck = std::move(a_deck);
        m_bank = std::move(a_bank);
    }

    // Load cards from a csv file
    void load_cards(std::string a_file);

    void shuffle() {
        // Create a Mersenne Twister random number generator
        std::mt19937 rng(std::random_device{}());

        for (auto& rows : m_deck.getMap()) {
            // Put all cards back into the deck
            auto& deck = rows.second;
            auto type = rows.first;
            auto& layout = m_options[type];
            while (layout.size()) {
                // replenish from deck
                deck.insert(deck.end(),
                            std::make_move_iterator(layout.end() - 1),
                            std::make_move_iterator(layout.end()));
                layout.erase(layout.end() - 1);
            }

            // Randomize the decks
            for (size_t i = 0; i < deck.size(); i++) {
                std::uniform_int_distribution<size_t> dist(i, deck.size() - 1);
                size_t r = dist(rng);
                std::swap(deck.at(i), deck.at(r));
            }
        }

        // Randomize the Nobles
        for (size_t i = 0; i < m_nobles.size(); i++) {
            std::uniform_int_distribution<size_t> dist(i, m_nobles.size() - 1);
            size_t r = dist(rng);
            std::swap(m_nobles.at(i), m_nobles.at(r));
        }
    }


    void prepare_for_turn() {
        // Make sure to replenish the board if possible
        for (auto& rows : m_options.getMap()) {
            auto& layout = rows.second;
            auto type = rows.first;
            auto& deck = m_deck[type];
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
