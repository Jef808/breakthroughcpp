#include "events.h"
#include "config.h"
#include "mcts.h"
#include <iostream>

void processEvents(sf::Window& window, breakthrough::Board& board)
{
    static breakthrough::MCTS mcts;
    static breakthrough::Move move = {-1, -1};

    static bool should_ponder = false;

    for (auto event = sf::Event{}; window.pollEvent(event);)
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Q)
            {
                window.close();
            }
        }
        else if (should_ponder)
        {
            mcts.ponder(board, 1000);
            move = mcts.choose_best(board);

            std::cout << "Computer move: " << "source: " << move.source << ", target: " << move.target << std::endl;

            board.play(move);
            should_ponder = false;
            move = {-1, -1};
        }
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePosition = {static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)};

                mousePosition -= (conf::board_position + conf::board_border_size);

                uint32_t file = mousePosition.x / conf::pawn_size.x;
                uint32_t row = 8 - mousePosition.y / conf::pawn_size.y;
                breakthrough::Square idx = 8 * row + file;

                breakthrough::Piece piece = board.at(idx);
                bool black_to_play = board.ply() & 1;

                if (move.source == -1)
                {
                    if ((is_black(piece) && black_to_play) || (is_white(piece) && !black_to_play))
                    {
                        move.source = idx;
                    }
                }
                else if (move.target == -1)
                {
                    move.target = idx;
                    std::cout << "Player move: " << "source: " << move.source << ", target: " << move.target << std::endl;
                    board.play(move);
                    should_ponder = true;
                }
            }
        }
    }
}
