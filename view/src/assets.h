#ifndef ASSETS_H_
#define ASSETS_H_

#include "config.h"
#include "board.h"
#include <SFML/Graphics.hpp>

class Assets
{
public:
    Assets();
    operator bool() const { return status; }
    const sf::Sprite& board() { return board_; }
    const sf::Sprite& white_pawn(breakthrough::Square square);
    const sf::Sprite& black_pawn(breakthrough::Square square);

private:
    bool status;
    sf::Texture board_texture;
    sf::Texture white_pawn_texture;
    sf::Texture black_pawn_texture;
    sf::Sprite board_;
    sf::Sprite white_pawn_;
    sf::Sprite black_pawn_;
};

#endif // ASSETS_H_
