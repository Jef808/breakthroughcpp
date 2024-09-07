/**
 * @file board.h
 *
 * The board representation of Breakthrough game is defined.
 * Pieces are represented by integers. In base 2, those are:
 * - 000: EMPTY (0)
 * - 010: WHITE (2)
 * - 100: BLACK (4)
 *
 * A Board instance is initialized with 16 white pawns in the
 * first two files, 16 black pawns in the last two, and the
 * other squares are left empty.
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <array>
#include <cstdint>
#include <iosfwd>
#include "zobrist.h"

namespace breakthrough {

/**
 * The three types of pieces in the game.
 */
enum Piece {
    EMPTY = 0,
    WHITE = 2,
    BLACK = 4,
};

/**
 * Helper function checking if a piece is empty.
 */
inline bool is_empty(Piece piece) {
    return not piece;
}

/**
 * Helper function checking if a piece is white.
 */
inline bool is_white(Piece piece) {
    return piece & 2;
}

/**
 * Helper function checking if a piece is black.
 */
inline bool is_black(Piece piece) {
    return piece & 4;
}

/**
 * Squares numbered from 0 to 63.
 *
 * The 'a1' square is 0 and 'h8' is 63
 */
using Square = int;

/**
 * Represents a move of the game.
 */
struct Move {
    /**
     * The square from which the piece is played.
     */
    Square source;

    /**
     * The square to which the piece is played.
     */
    Square target;
};

class Board {
public:
    /**
     * Initialize a new game.
     */
    Board();

    /**
     * Create a game with position given by a fen string.
     */
    explicit Board(std::istream& fen);

    /**
     * Play the given move.
     *
     * This method does not check if the move is valid.
     */
    void play(Move move);

    /**
     * Check if the game is over.
     */
    bool is_terminal() const;

    /**
     * Const access to the piece at the given square.
     */
    Piece at(Square square) const { return m_squares[square]; }

    /**
     * Const access to the current ply.
     */
    int ply() const { return m_ply; }

    /**
     * Get the value of the hash for the current position.
     */
    uint64_t hash() const { return m_hash; }

    /**
     * Construct the fen string of the current position.
     */
    std::string fen() const;

private:
    /**
     * The piece value of each square of the board.
     */
    std::array<Piece, 64> m_squares;

    /**
     * Used for updating the hash value.
     */
    Zobrist m_zobrist{};

    /**
     * The hash value for the current position.
     */
    uint64_t m_hash{0};

    /**
     * The current ply (half-move) of the game.
     */
    int m_ply{0};
};

}  // namespace breakthrough

#endif // BOARD_H_
