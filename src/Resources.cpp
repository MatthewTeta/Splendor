#include "Resources.hpp"

// write constructor for CardDeck
Splendor::CardDeck::CardDeck() {
    // initialize card deck with the 40 cards from assets folder

    std::ifstream card_file("../assets/cards.txt");
    std::string line;
    std::vector<std::string> card_lines;
    while (std::getline(card_file, line)) {
        card_lines.push_back(line);
    }
    // 2. parse the file
    // example of card line  ONE,0,3,0,0,0,0,Diamond

    // 3. create the cards
    for (auto card_line : card_lines) {
        std::vector<std::string> card_line_tokens;
        std::stringstream ss(card_line);
        std::string token;
        while (std::getline(ss, token, ',')) {
            card_line_tokens.push_back(token);
        }
        // 4. create the card
        Splendor::Card::Type card_type;
        if (card_line_tokens[0] == "ONE") {
            card_type = Splendor::Card::Type::ONE;
        } else if (card_line_tokens[0] == "TWO") {
            card_type = Splendor::Card::Type::TWO;
        } else {
            card_type = Splendor::Card::Type::THREE;
        }
        int points = std::stoi(card_line_tokens[1]);
        Splendor::Gemstone token_type;
        if (card_line_tokens[7] == "Diamond") {
            token_type = Splendor::Gemstone::Diamond;
        } else if (card_line_tokens[7] == "Emerald") {
            token_type = Splendor::Gemstone::Emerald;
        } else if (card_line_tokens[7] == "Onyx") {
            token_type = Splendor::Gemstone::Onyx;
        } else if (card_line_tokens[7] == "Ruby") {
            token_type = Splendor::Gemstone::Ruby;
        } else {
            token_type = Splendor::Gemstone::Sapphire;
        }
        std::map<Splendor::Gemstone, int> card_cost;
        card_cost[Splendor::Gemstone::Diamond] = std::stoi(card_line_tokens[2]);
        card_cost[Splendor::Gemstone::Emerald] = std::stoi(card_line_tokens[3]);
        card_cost[Splendor::Gemstone::Onyx] = std::stoi(card_line_tokens[4]);
        card_cost[Splendor::Gemstone::Ruby] = std::stoi(card_line_tokens[5]);
        card_cost[Splendor::Gemstone::Sapphire] = std::stoi(card_line_tokens[6]);
        Splendor::Card card(card_type, points, token_type, card_cost);
        this->card_deck.push_back(card);
    }


}