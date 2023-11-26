/**
 * Author: Matthew Teta
 * Date: 11/25/2023
 *
* File: Game.hpp
 */

#include "Pieces.hpp"
#include "Board.hpp"
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <variant>

namespace Splendor {

// enum ActionType {
//     TAKE_THREE,
//     TAKE_TWO,
//     PURCHASE,
//     RESERVE,
// } m_action_type;

struct TakeThree {
    std::array<Token::Type, 3> token_types;
};

struct TakeTwo {
    Token::Type token_type;
};

struct Reserve {
    Card &card;
};

struct Purchase {
    Card &card;
};

// This allows us to use the std::visit to check which variant we have...
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
using Action = std::variant<std::monostate, TakeThree, TakeTwo, Reserve, Purchase>;


class Game;
using PlayerFunc = std::function<const Action(const Game &)>;

class Player {
public:
    std::vector<Token> m_tokens;
    std::vector<Card>  m_cards;
    std::vector<Card>  m_reserve_cards;
    const PlayerFunc   m_play;
    
    Player(const PlayerFunc a_play) : m_play(a_play) {};

    const Action play(const Game& a_game) {
        return m_play(a_game);
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
            std::visit(overload {
                [&](std::monostate&) { throw std::domain_error("This should not be possible.") },
                [&](TakeThree& m) {

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
                        p.m_tokens.insert(p.m_tokens.end(), m_board.m_bank.end() - 1, m_board.m_bank.end());
                        m_board.m_bank.erase(m_board.m_bank.end() - 1);
                    }

                },
                [&](TakeTwo& m) {

                    // Check move validity
                    // Do we have enough tokens
                    if (m_board.m_bank[m.token_type].size() < 4) {
                        valid = false;
                        return;
                    }

                    // Perform the action
                    p.m_tokens.insert(p.m_tokens.end(), m_board.m_bank.end() - 2, m_board.m_bank.end());
                    m_board.m_bank.erase(m_board.m_bank.end() - 2);

                },
                [&](Reserve& m) {

                    // Check move validity
           

                },
                [&](Purchase& m) {}
            }, a);
        } while (!valid);
    }

};

}

