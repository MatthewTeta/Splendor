/**
 * Author: Matthew Teta
 * Date: 11/25/2023
 *
 * File: main.cpp
 */

#include <iostream>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/component/screen_interactive.hpp"   // for Component, ScreenInteractive
#include "ftxui/component/captured_mouse.hpp"       // for ftxui
#include "ftxui/component/component.hpp"            // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"       // for ComponentBase

#include "Pieces.hpp"
#include "Board.hpp"
#include "Game.hpp"

#define CARD_HEIGHT 10
#define CARD_WIDTH 20
#define NOBLE_HEIGHT 7

// using token_text_t = struct {
//
// };
// static const std::map<Token::Type, token_text_t> token_map = {};

int main(void) {
    using namespace ftxui;
    using namespace Splendor;

    // Create the board
    Board board;
    board.load_cards("../Splendor_Cards.csv");
    board.shuffle();

    auto pos_text = [](V2 pos) {
        return text(" " + std::to_string(pos.row) + ", " + std::to_string(pos.col) + " ");
    };

    auto token_text = [&](Token::Type t, int num) {
        std::string n_str = std::to_string(num);
        switch (t) {
            case Token::Type::Emerald:
                return hbox({text(L" Emerald:  "), text(n_str) | bold, text(" ")}) | bgcolor(Color::Green);
            case Token::Type::Sapphire:
                return hbox({text(L" Sapphire: "), text(n_str) | bold, text(" ")}) | bgcolor(Color::Blue);
            case Token::Type::Ruby:
                return hbox({text(L" Ruby:     "), text(n_str) | bold, text(" ")}) | bgcolor(Color::Red);
            case Token::Type::Diamond:
                return hbox({text(L" Diamond:  "), text(n_str) | bold, text(" ")}) | bgcolor(Color::Cyan);
            case Token::Type::Onyx:
                return hbox({text(L" Onyx:     "), text(n_str) | bold, text(" ")}) | bgcolor(Color::GrayDark);
            case Token::Type::Gold:
                return hbox({text(L" Gold:     "), text(n_str) | bold, text(" ")}) | bgcolor(Color::YellowLight);
        default:
            return hbox({text(L"Error: Invalid Token")});
        }
    };

    auto deck = [&](int num, V2 pos) {
        auto centeredNumber = hbox({
            filler(),
            text(std::to_string(num)) | hcenter,
            filler(),
        });

        auto content = vbox({
            filler(),
            centeredNumber | vcenter,
            filler(),
        });

        return window(pos_text(pos), content)
            | size(WIDTH, EQUAL, CARD_WIDTH)
            | size(HEIGHT, EQUAL, CARD_HEIGHT);
    };

    auto card = [&](Card& c, V2 pos) {
        std::vector<Element> children = {
            hbox({text(L"Points:    "), text(std::to_string(c.m_points)) | bold}) | color(Color::White),
            separator(),
        };
        for (auto it : c.m_cost) {
            if (0 == it.first) continue;
            children.push_back(token_text(it.second, it.first));
        }
        auto content = vbox(children);
        return window(pos_text(pos), content)
        | size(WIDTH, EQUAL, CARD_WIDTH)
        | size(HEIGHT, EQUAL, CARD_HEIGHT);
    };

    auto noble = [&](const Noble& n) {
        std::vector<Element> children = {
            hbox({text(L"Points:    "), text(std::to_string(n.m_points)) | bold}) | color(Color::White),
            separator(),
        };
        for (auto it : n.m_cost) {
            if (0 == it.first) continue;
            children.push_back(token_text(it.second, it.first));
        }
        auto content = vbox(children);
        return window(text(L" Noble "), content)
        | size(WIDTH, EQUAL, CARD_WIDTH)
        | size(HEIGHT, EQUAL, NOBLE_HEIGHT);

    };

    auto bank = [&](Bank& b) {
        std::vector<Element> children;
        for (auto it : b.getMap()) {
            children.push_back(token_text(it.first, it.second.size()));
        }
        auto content = vbox(children);
        return window(text(L" Bank "), content)
        | size(WIDTH, EQUAL, CARD_WIDTH * 5);
    };

    auto draw_board = [&](Board& b) {
        std::vector<Element> root;

        // Render 5 Nobles
        std::vector<Element> nobles;
        for (auto it : b.getNobles()) {
            nobles.push_back(noble(it));
        }
        root.push_back({hbox(nobles)});

        // Render 3 Rows of cards
        std::vector<Element> cards;
        int y = 0;
        for (auto it : b.getCards()) {
            std::vector<Element> row = {
                vbox(deck(static_cast<int>(it.first) + 1, {++y, 0}))
            };
            int x = 1;
            for (auto it2 : it.second) {
                row.push_back(vbox(card(it2, {y, x++})));
            }
            cards.insert(cards.begin(),
            {
                hbox(row),
            });
        }
        root.push_back(vbox(cards));

        // Render Bank
        root.push_back(vbox(bank(b.getBank())));

        return vbox(root);
    };

  auto summary = [&] {
    auto content = vbox({
        hbox({text(L"- done:   "), text(L"3") | bold}) | color(Color::Green),
        hbox({text(L"- active: "), text(L"2") | bold}) | color(Color::RedLight),
        hbox({text(L"- queue:  "), text(L"9") | bold}) | color(Color::Red),
    });
    return window(text(L" Summary "), content);
  };


  auto document = draw_board(board);

  // Limit the size of the document to 80 char.
  document = document | size(WIDTH, LESS_THAN, 80);

//   auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
//   Render(screen, document);

//   std::cout << screen.ToString() << '\0' << std::endl;

    // Interactive
    std::string s1 = "test";
    auto layout = Input(&s1);
    auto renderer = Renderer(layout, [&] { return draw_board(board); });
    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);

  return EXIT_SUCCESS;
}
