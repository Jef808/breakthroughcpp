#include <algorithm>
#include <cassert>
#include <charconv>
#include <iostream>
#include <sstream>
#include <string>
#include <optional>
#include <system_error>
#include "board.h"

namespace breakthrough {

namespace {

constexpr auto to_int = [](std::string_view sv) -> std::optional<int> {
    int value{};
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{}) {
        return value;
    }
    return std::nullopt;
};

inline int hash_key(Square square, Piece piece) {
    return (square << 2) ^ piece;
}

}  // namespace

Board::Board() {
    std::fill_n(m_squares.begin(), 16, Piece::WHITE);
    std::fill_n(m_squares.begin() + 16, 32, Piece::EMPTY);
    std::fill_n(m_squares.rbegin(), 16, Piece::BLACK);

    // Initialize the zobrist hash table
    for (int i = 0; i < 64; ++i) {
        m_zobrist.generate(hash_key(i, Piece::WHITE));
        m_zobrist.generate(hash_key(i, Piece::BLACK));
    }

    // Initialize the position hash
    for (int i = 0; i < 16; ++i) {
        m_hash ^= m_zobrist[hash_key(i, Piece::WHITE)];
        m_hash ^= m_zobrist[hash_key(63 - i, Piece::BLACK)];
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

    for (int i = 0; i < 64; ++i) {
        m_hash ^= hash_key(i, m_squares[i]);
    }

    std::getline(fen, buf, ' ');
    auto black_to_play = buf[0] == 'b';

    fen.read(buf.data(), 6);

    int full_moves;
    fen >> full_moves;

    m_ply = (full_moves - 1) * 2 + black_to_play;
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
    m_hash ^= m_zobrist[hash_key(move.source, m_squares[move.source])];
    if (!is_empty(m_squares[move.target])) {
        m_hash ^= m_zobrist[hash_key(move.target, m_squares[move.target])];
    }
    m_hash ^= m_zobrist[hash_key(move.target, m_squares[move.source])];

    // Update the board
    m_squares[move.target] = m_squares[move.source];
    m_squares[move.source] = Piece::EMPTY;
    ++m_ply;
}

bool Board::is_terminal() const {
    const bool black_to_play = m_ply & 1;
    return (black_to_play && std::any_of(m_squares.end() - 8, m_squares.end(), is_white)) ||
        (!black_to_play && std::any_of(m_squares.begin(), m_squares.begin() + 8, is_black));
}

}  // namespace breakthrough
