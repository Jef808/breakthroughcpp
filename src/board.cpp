#include <algorithm>
#include <cassert>
#include <charconv>
#include <iostream>
#include <sstream>
#include <string>
#include <optional>
#include <system_error>
#include "board.h"
#include "zobrist.h"

namespace breakthrough {

namespace {

constexpr auto to_int = [](std::string_view sv) -> std::optional<int> {
    int value{};
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{}) {
        return value;
    }
    return std::nullopt;
};

/**
 * Used for updating the hash value.
 */
Zobrist zobrist;

inline uint64_t get_hash(Square square, Piece piece) {
    if (is_empty(piece)) {
        return 0;
    }
    uint64_t value = zobrist[(square << 3) ^ piece];
    assert(value != 0 && "Zobrist value is 0");

    return value;
}

void init_zobrist() {
    for (int i = 0; i < 64; ++i) {
        zobrist.generate((i << 3) ^ Piece::WHITE);
        zobrist.generate((i << 3) ^ Piece::BLACK);
    }

    assert(zobrist.size() == 128 && "Zobrist table not of size 128 after initialization");
}

}  // namespace

Board::Board() {
    std::fill_n(m_squares.begin(), 16, Piece::WHITE);
    std::fill_n(m_squares.begin() + 16, 32, Piece::EMPTY);
    std::fill_n(m_squares.rbegin(), 16, Piece::BLACK);

    if (zobrist.size() == 0) {
        init_zobrist();
    }

    // Initialize the position hash
    for (int i = 0; i < 16; ++i) {
        m_hash ^= get_hash(i, Piece::WHITE);
        m_hash ^= get_hash(63 - i, Piece::BLACK);
    }
}

Board::Board(std::istream& fen) {
    std::string buf;
    std::getline(fen, buf, ' ');

    auto square = m_squares.begin();
    for (auto c : buf) {
        if (c == '/') {
            continue;
        }
        if (c == 'P') {
            *square++ = Piece::WHITE;
        } else if (c == 'p') {
            *square++ = Piece::BLACK;
        } else {
            for (auto i = 0; i < to_int(&c); ++i) {
                *square++ = Piece::EMPTY;
            }
        }
    }

    std::getline(fen, buf, ' ');
    auto black_to_play = buf[0] == 'b';

    fen.read(buf.data(), 6);

    int full_moves;
    fen >> full_moves;

    m_ply = (full_moves - 1) * 2 + black_to_play;

    if (zobrist.size() == 0) {
        init_zobrist();
    }

    for (int i = 0; i < 64; ++i) {
        m_hash ^= get_hash(i, m_squares[i]);
    }
}

std::string Board::fen() const {
    std::stringstream ss;

    auto square = m_squares.begin();
    for (int rank = 0; rank < 8; ++rank) {
        auto end = square + 8;
        auto empty_count = 0;

        for (; square != end; ++square) {
            if (*square == Piece::WHITE) {
                if (empty_count) {
                    ss << empty_count;
                }
                ss << 'P';
                empty_count = 0;
            }
            else if (*square == Piece::BLACK) {
                if (empty_count) {
                    ss << empty_count;
                }
                ss << 'p';
                empty_count = 0;
            }
            else {
                ++empty_count;
            }
        }
        if (square != m_squares.end()) {
            ss << '/';
        }
    }
    auto black_to_play = m_ply & 1;

    ss << (black_to_play ? " b" : " w");
    ss << " - - 0 ";
    ss << int(m_ply / 2) + 1;

    return ss.str();
}

void Board::play(Move move) {
    // Update the hash value
    m_hash ^= get_hash(move.source, m_squares[move.source]);
    m_hash ^= get_hash(move.target, m_squares[move.target]);
    m_hash ^= get_hash(move.target, m_squares[move.source]);

    // Update the board
    m_squares[move.target] = m_squares[move.source];
    m_squares[move.source] = Piece::EMPTY;

    // Increment the ply
    ++m_ply;
}

bool Board::is_terminal() const {
    const bool black_to_play = m_ply & 1;
    return (black_to_play && std::any_of(m_squares.end() - 8, m_squares.end(), is_white)) ||
        (!black_to_play && std::any_of(m_squares.begin(), m_squares.begin() + 8, is_black));
}

}  // namespace breakthrough
