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

namespace Splendor {

struct Action {

    enum Type {
        TAKE_THREE,
        TAKE_TWO,
        PURCHASE,
        RESERVE,
    } m_action_type;

    struct TakeThree {
        Token::Type token_types[3];
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

};


class Game {
public:
using PlayerFuncs = std::function<Action(const Game &)>;

    Board m_board;
    std::vector<Player> &m_players;
    uint8_t m_turn; // Who's turn is it? (index)

    Game(const std::vector<const PlayerFuncs> &a_player_functions) {
        m_board = Board();
        for (PlayerFuncs pf : a_player_functions) {
            m_players.push_back(Player(pf));
        }
    }

    bool next_turn() {
        // Current player whose turn it is
        Player &p = m_players[m_turn++];
        m_turn %= m_players.size();

        // Have the player make a move
        Action &a = p.play(m_board, p);
    }

};

}

