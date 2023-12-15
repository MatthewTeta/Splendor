/**
 * Author: Matthew Teta
 * Date: 12/15/2023
 *
 * File: load_cards.hpp
 */

#ifndef LOAD_CARDS_HPP
#define LOAD_CARDS_HPP


#include "Board.hpp"
#include "rapidcsv.h"
#include <vector>

using namespace Splendor;

void Board::load_cards(std::string a_file) {
    // Open the file
    rapidcsv::Document doc(a_file);

    // Each row has one card or noble
    // Headers are "Level,Diamond,Sapphire,Emerald,Ruby,Onyx,points,type"
    std::vector<std::string> headers = doc.GetColumnNames();
    size_t N = doc.GetRowCount();
    for (size_t row_idx = 0; row_idx < N; row_idx++) {
        // Get the row
        std::vector<std::string> row = doc.GetRow<std::string>(row_idx);

        // Get the cost
        std::vector<std::pair<int, Token::Type>> cost;
        for (int i = 1; i < 6; i++) {
            int num = std::stoi(row.at(i));
            Token::Type type = Token::type_from_str(headers.at(i));
            cost.push_back(std::make_pair(num, type));
        }

        // Get the Points
        int points = std::stoi(row.at(6));

        // Get the level
        std::string level_str = row.at(0);
        if (level_str == "Noble") {
            Noble noble(points, cost);

            // Add the noble to the deck
            m_nobles.push_back(noble);
        } else {
            Card::Type level = Card::type_from_str(level_str);

            std::string type_str = row.at(7);
            Token::Type type = Token::type_from_str(type_str);

            Card card(level, type, points, cost);

            // Add the card to the deck
            m_deck[level].push_back(card);
        }
    }
}


#endif // LOAD_CARDS_HPP
