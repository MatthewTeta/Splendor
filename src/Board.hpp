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
#include <ranges>
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

static const std::map<int, Card::Type> ROW_MAP = {
    { 0, Card::Type::ONE },
    { 1, Card::Type::TWO },
    { 2, Card::Type::THREE },
};

class Board {
private:
    Deck m_deck;
    std::vector<Noble> m_nobles;
    Bank m_bank;
public:

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

    auto getNobles() {
        return m_nobles | std::views::take(5);
    }

    auto getCardLevel(Card::Type a_type) {
        return m_deck[a_type] | std::views::take(4);
    }

    auto getCards() {
        return std::map<Card::Type, decltype(getCardLevel(Card::Type::ONE))>{
            {Card::Type::ONE, getCardLevel(Card::Type::ONE)},
            {Card::Type::TWO, getCardLevel(Card::Type::TWO)},
            {Card::Type::THREE, getCardLevel(Card::Type::THREE)},
        };
    }

    Bank& getBank() {
        return m_bank;
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

    // Tries to copy the card at pos into the a_card_out param for inspection
    bool getCardAtPosition(V2 pos, Card& a_card_out) {

        if (pos.row >= 3 || pos.row < 0 || pos.col >= 4 || pos.col < 0) return false;

        Card::Type t = ROW_MAP.at(pos.row);

        if (pos.col == 0) {
            // Get from top of the deck
            if (m_deck[t].size() <= 4) return false;
            a_card_out = m_deck[t].at(0);
        } else {
            // Get from cards layed out face up
            if (pos.col <= m_deck[t].size()) return false;
            a_card_out = m_deck[t].at(pos.col);
        }

        return true;
    }

    // Tries to move the card at the position out of the board / deck and into the a_card_out param
    bool takeCardAtPosition(V2 pos, Card& a_card_out) {

        if (pos.row >= 3 || pos.row < 0 || pos.col >= 4 || pos.col < 0) return false;

        Card::Type t = ROW_MAP.at(pos.row);

        if (pos.col == 0) {
            // Get from top of the deck
            if (m_deck[t].size() <= 4) return false;
            a_card_out = m_deck[t].at(0);
            m_deck[t].erase(m_deck[t].begin());
        } else {
            // Get from cards layed out face up
            if (pos.col <= m_deck[t].size()) return false;
            a_card_out = m_deck[t].at(pos.col);
            m_deck[t].erase(m_deck[t].begin() + pos.col);
        }

        return true;
    }

};

}

#endif
