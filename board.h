/**
 * @file board.h
 *
 * The board representation of Breakthrough game is defined.
 * Pieces are represented by integers. In base 2, those are:
 * - 0: EMPTY
 * - 2: WHITE
 * - 4: BLACK
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <array>
#include <cstdint>

enum Piece {
    EMPTY = 0,
    WHITE = 2,
    BLACK = 4,
};

inline bool is_empty(Piece piece) {
    return not piece;
}

inline bool is_white(Piece piece) {
    return piece & 2;
}

inline bool is_black(Piece piece) {
    return piece & 4;
}

using Square = int;

struct Move {
    Square source;
    Square target;
};

class Board {
public:
    Board();

    Piece at(Square square) const;

private:
    std::array<Piece, 64> m_squares;
};


#endif // BOARD_H_
