/**
 * Author: Matthew Teta
 * Date: 12/15/2023
 *
 * File: splendor.hpp
 */

#ifndef SPLENDOR_HPP
#define SPLENDOR_HPP

#include <string>
#include <vector>
#include <iterator>
#include <map>
#include <functional>
#include <variant>


namespace Splendor {

// Declarations
struct Token;
struct Card;
struct Noble;
struct Deck;
struct Bank;
class Player;
class Board;
class Game;
class Action;
using PlayerFunc = std::function<const Action(const Game &)>;


// Definitions
struct Token {
    enum Type {
        Emerald,
        Sapphire,
        Ruby,
        Diamond,
        Onyx,
        Gold,
    };
    static Type type_from_str(const std::string& type_str) {
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

struct Card : public Token {
    enum Type {
        ONE,
        TWO,
        THREE,
    };
    static Type type_from_str(std::string& level_str) {
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

struct Noble {

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

struct Deck {
private:
    using _Deck = std::map<Card::Type, std::vector<Card>>;
    _Deck _this;

public:
    Deck() {
        // Insert all card types
        _this.insert(Card::Type::ONE, {});
        _this.insert(Card::Type::TWO, {});
        _this.insert(Card::Type::THREE, {});
    }

    Deck& operator+=(Card& rhs) {
        _this.at(rhs.m_card_type).push_back(rhs);
        return *this;
    }

    std::vector<Card>& operator[](Card::Type a_type) {
        return _this.at(a_type);
    }
};

struct Bank {
    using _Bank = std::map<Token::Type, std::vector<Token>>;
    _Bank _this;

public:
    Bank() {
        // Insert all card types
        _this.insert(Token::Type::Emerald, {});
        _this.insert(Token::Type::Sapphire, {});
        _this.insert(Token::Type::Ruby, {});
        _this.insert(Token::Type::Diamond, {});
        _this.insert(Token::Type::Onyx, {});
        _this.insert(Token::Type::Gold, {});
    }

    Bank& operator+=(Token& rhs) {
        _this.at(rhs.m_token_type).push_back(rhs);
        return *this;
    }

    std::vector<Token>& operator[](Token::Type a_type) {
        return _this.at(a_type);
    }
};

class Player {
public:
    std::vector<Token> m_tokens;
    std::vector<Card>  m_cards;
    std::vector<Card>  m_reserve_cards;
    const PlayerFunc   m_play;

    Player(const PlayerFunc a_play) : m_play(a_play) {};

    const Action play(const Game& a_game);

};

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
        Bank &a_bank
    ) {
        m_nobles = std::move(a_nobles);
        m_deck = std::move(a_deck);
        m_bank = std::move(a_bank);
    }

    // Load cards from a csv file
    void load_cards(std::string a_file);

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

class Game {
public:

    Board m_board;
    std::vector<Player> m_players;
    uint8_t m_turn; // Who's turn is it? (index)

    Game(const std::vector<PlayerFunc> &a_player_functions) {

        std::vector<Noble> nobles = {
            {
                3,
                {
                    { 3, Token::Type::Ruby },
                    { 3, Token::Type::Onyx },
                    { 3, Token::Type::Emerald },
                }
            },
            {
                3,
                {
                    { 3, Token::Type::Onyx },
                    { 3, Token::Type::Sapphire },
                    { 3, Token::Type::Emerald },
                },
            },
            {
                3,
                {
                    { 3, Token::Type::Diamond },
                    { 3, Token::Type::Onyx },
                    { 3, Token::Type::Ruby },
                }
            },
            {
                3,
                {
                    { 4, Token::Type::Diamond },
                    { 4, Token::Type::Emerald },
                }
            },
            {
                3,
                {
                    { 4, Token::Type::Ruby },
                    { 4, Token::Type::Sapphire },
                }
            },
        };

        Deck deck = {
            {
                Card::Type::ONE,
                {
                    {
                        Card::Type::ONE,
                        Token::Type::Diamond,
                        0,
                        {
                            { 2, Token::Type::Emerald },
                            { 1, Token::Type::Diamond },
                            { 1, Token::Type::Onyx },
                            { 1, Token::Type::Sapphire },
                        }
                    },
                    {
                        Card::Type::ONE,
                        Token::Type::Emerald,
                        0,
                        {
                            { 1, Token::Type::Emerald },
                            { 1, Token::Type::Diamond },
                            { 3, Token::Type::Ruby },
                        }
                    },
                    {
                        Card::Type::ONE,
                        Token::Type::Onyx,
                        0,
                        {
                            { 2, Token::Type::Sapphire },
                            { 2, Token::Type::Onyx },
                        }
                    },
                    {
                        Card::Type::ONE,
                        Token::Type::Ruby,
                        0,
                        {
                            { 1, Token::Type::Emerald },
                            { 2, Token::Type::Diamond },
                            { 1, Token::Type::Onyx },
                            { 1, Token::Type::Sapphire },
                        }
                    },
                    {
                        Card::Type::ONE,
                        Token::Type::Sapphire,
                        0,
                        {
                            { 3, Token::Type::Ruby },
                            { 2, Token::Type::Sapphire },
                        }
                    },
                }
            },
        };

        std::map<Token::Type, std::vector<Token>> tokens = {
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

        m_board = Board(nobles, deck, tokens);
        m_board.shuffle();
        for (const PlayerFunc pf : a_player_functions) {
            m_players.push_back(Player(pf));
        }
    }

    void next_turn() {
        // Current player whose turn it is
        Player &p = m_players[m_turn++];
        m_turn %= m_players.size();

        m_board.prepare_for_turn();

        // Have the player make a move
        bool valid = true;
        do {
            const Action a = p.play(*this);

            // Handle the move
            std::visit(overloaded{
                [&](TakeThree m) {

                    // Check move validity
                    if (
                        m.token_types[0] == m.token_types[1] ||
                        m.token_types[0] == m.token_types[2] ||
                        m.token_types[1] == m.token_types[2]
                    ) {
                        valid = false;
                        return;
                    }

                    // Do we have enough tokens
                    for (auto type : m.token_types) {
                        if (!m_board.m_bank[type].size()) {
                            valid = false;
                            return;
                        }
                    }

                    // Perform the action
                    for (auto type : m.token_types) {
                        // p.m_tokens.insert(p.m_tokens.end(), m_board.m_bank.end() - 1, m_board.m_bank.end());
                        // m_board.m_bank.erase(m_board.m_bank.end() - 1);
                        p.m_tokens.push_back(Token(type));
                        m_board.m_bank[type].pop_back();
                    }

                },
                [&](TakeTwo m) {

                    // Check move validity
                    // Do we have enough tokens
                    if (m_board.m_bank[m.token_type].size() < 4) {
                        valid = false;
                        return;
                    }

                    // Perform the action
                    // p.m_tokens.insert(p.m_tokens.end(), m_board.m_bank.end() - 2, m_board.m_bank.end());
                    // m_board.m_bank.erase(m_board.m_bank.end() - 2);

                },
                [&](Reserve m) {

                    // Check move validity


                },
                [&](Purchase m) {}
            }, a);
        } while (!valid);
    }

};


};


#endif // SPLENDOR_HPP
