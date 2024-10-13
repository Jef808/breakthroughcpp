#include "assets.h"

Assets::Assets() : status{true}
{
    status &= board_texture.loadFromFile(conf::board_asset_file);
    status &= white_pawn_texture.loadFromFile(conf::wpawn_asset_file);
    status &= black_pawn_texture.loadFromFile(conf::bpawn_asset_file);

    board_.setTexture(board_texture);
    board_.setPosition(conf::board_position);

    white_pawn_.setTexture(white_pawn_texture);
    black_pawn_.setTexture(black_pawn_texture);
}

namespace
{

sf::Vector2f get_position(breakthrough::Square square)
{
    float row = static_cast<int>(square / 8);
    float file = square % 8;
    sf::Vector2f position = {
        file * conf::pawn_size.x,
        (7 - row) * conf::pawn_size.y - 1.f
    };
    return position + conf::board_border_size + conf::board_position;
}

}

const sf::Sprite& Assets::white_pawn(breakthrough::Square square)
{
    sf::Vector2f position = get_position(square);
    white_pawn_.setPosition(position);
    return white_pawn_;
}

const sf::Sprite& Assets::black_pawn(breakthrough::Square square)
{
    sf::Vector2f position = get_position(square);
    black_pawn_.setPosition(position);
    return black_pawn_;
}
