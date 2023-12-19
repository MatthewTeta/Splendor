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
    V2 card_pos;
};

struct Purchase {
    V2 card_pos;
};

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
/* template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>; */
using Action = std::variant<TakeThree, TakeTwo, Reserve, Purchase>;


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

    Game(const Board& a_board, const std::vector<PlayerFunc> &a_player_functions) {
        m_board = std::move(a_board);
        for (const PlayerFunc pf : a_player_functions) {
            m_players.push_back(Player(pf));
        }
    }

    void next_turn() {
        // Current player whose turn it is
        Player &p = m_players[m_turn++];
        m_turn %= m_players.size();
        auto bank = m_board.getBank();

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
                        if (!bank[type].size()) {
                            valid = false;
                            return;
                        }
                    }

                    // Perform the action
                    for (auto type : m.token_types) {
                        // p.m_tokens.insert(p.m_tokens.end(), bank.end() - 1, bank.end());
                        // bank.erase(bank.end() - 1);
                        p.m_tokens.push_back(Token(type));
                        bank[type].pop_back();
                    }

                },
                [&](TakeTwo m) {

                    // Check move validity
                    // Do we have enough tokens
                    if (bank[m.token_type].size() < 4) {
                        valid = false;
                        return;
                    }

                    // Perform the action
                    p.m_tokens.push_back(Token(m.token_type));
                    p.m_tokens.push_back(Token(m.token_type));
                    bank[m.token_type].pop_back();
                    bank[m.token_type].pop_back();

                },
                [&](Reserve m) {

                    // Check move validity
                    if (p.m_reserve_cards.size() >= 3) {
                        valid = false;
                        return;
                    }

                    Card card;
                    if (m_board.takeCardAtPosition(m.card_pos, card)) {
                        p.m_reserve_cards.push_back(card);
                        if (bank[Token::Type::Gold].size()) {
                            p.m_tokens.push_back(Token(Token::Type::Gold));
                        }
                    } else {
                        valid = false;
                    }

                },
                [&](Purchase m) {

                    // Check move validity
                    Card card;
                    if (m_board.getCardAtPosition(m.card_pos, card)) {
                        // Check that player can purchase this card
                        auto cost = card.m_cost;
                        Player np = p;
                        // First check the players hand for cards of each gem type
                        for (auto x : p.m_cards) {
                            for (auto& c : cost) {
                                if (0 == c.first) break;
                                if (x.m_token_type == c.second) c.first--;
                            }
                        }
                        // Then check the gems to pay for the card
                        std::vector<Token::Type> tokens_used;
                        for (auto x : p.m_tokens) {
                            for (auto& c : cost) {
                                if (0 == c.first) break;
                                if (x.m_token_type == c.second) {
                                    c.first--;
                                    tokens_used.push_back(x.m_token_type);
                                }
                            }
                        }
                        // Check if any costs are unsatisfied
                        bool purchased = true;
                        for (auto c : cost) {
                            if (c.first) {
                                purchased = false;
                                break;
                            }
                        }
                        if (!purchased) {
                            valid = false;
                            return;
                        }
                        // Move User tokens to the bank
                        for (auto t : tokens_used) {
                            int i = 0;
                            for (auto x : np.m_tokens) {
                                if (x.m_token_type == t) {
                                    np.m_tokens.erase(np.m_tokens.begin() + i);
                                    bank[t].push_back(Token(t));
                                    break;
                                }
                                i++;
                            }
                        }
                        // Actually take card out of the board and put it into the users hand
                        m_board.takeCardAtPosition(m.card_pos, card);
                        p.m_cards.push_back(card);
                    } else {
                        valid = false;
                    }

                }
            }, a);
        } while (!valid);
    }

};

}
