#ifndef CONFIG_H_
#define CONFIG_H_

#include <SFML/System/Vector2.hpp>
#include <cstdint>

namespace conf
{
    // Window configuration
    const sf::Vector2u window_size = {960, 1080};
    const sf::Vector2f window_size_f = static_cast<sf::Vector2f>(window_size);
    const uint32_t max_frame_rate = 144;

    // Assets configuration
    const auto board_asset_file = "view/res/board.png";
    const auto wpawn_asset_file = "view/res/wpawn.png";
    const auto bpawn_asset_file = "view/res/bpawn.png";
    const sf::Vector2f board_size = {390.0f, 390.0f};
    const sf::Vector2f pawn_size = {45.0f, 45.0f};
    const sf::Vector2f board_border_size = {15.0f, 15.0f};

    // Board configuration
    const sf::Vector2f board_position = (window_size_f - board_size) * 0.5f;
}

#endif // CONFIG_H_
