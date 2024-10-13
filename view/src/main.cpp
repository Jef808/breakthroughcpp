#include <SFML/Graphics.hpp>
#include "events.h"
#include "config.h"
#include "assets.h"
#include "board.h"
#include <iostream>
#include <vector>

int main()
{
    auto window = sf::RenderWindow{ { 1920u, 1080u }, "CMake SFML Project" };
    window.setFramerateLimit(conf::max_frame_rate);

    Assets assets;
    if (!assets)
    {
        std::cerr << "Failed to load assets" << std::endl;
        return EXIT_FAILURE;
    }

    breakthrough::Board game;

    while (window.isOpen())
    {
        processEvents(window, game);

        window.clear(sf::Color{120, 120, 120});
        window.draw(assets.board());

        for (breakthrough::Square i = 0; i < 64; ++i)
        {
            if (is_white(game.at(i)))
            {
                const sf::Sprite& white_pawn = assets.white_pawn(i);
                window.draw(white_pawn);
            }
            else if (is_black(game.at(i)))
            {
                const sf::Sprite& black_pawn = assets.black_pawn(i);
                window.draw(black_pawn);
            }
        }

        window.display();
    }
}
