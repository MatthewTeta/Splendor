
#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <map>
#include "Pieces.hpp"

namespace Splendor {

class Mine {
    private:
        std::map<Gemstone, int> mine_state;
    public:
        Mine() = default;
};


template <typename GenericDeckMember> 
class Deck {
    private:
        std::vector<GenericDeckMember> deck;
    public:
        Deck() = default;
};

class CardDeck : public Deck<Splendor::Card> {
    private:
        std::vector<Splendor::Card> card_deck;
    public:
        CardDeck() = default;
};

class NobleDeck : public Deck<Splendor::Noble> {
    private:
        std::vector<Splendor::Noble> noble_deck;
    public:
        NobleDeck() = default;
};

}


#endif