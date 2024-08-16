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

Board::Board() {
    std::fill_n(m_squares.begin(), 16, Piece::WHITE);
    std::fill_n(m_squares.begin() + 16, 32, Piece::EMPTY);
    std::fill_n(m_squares.rbegin(), 16, Piece::BLACK);
}

namespace {

constexpr auto to_int = [](std::string_view sv) -> std::optional<int> {
    int value{};
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{}) {
        return value;
    }
    return std::nullopt;
};

}  // namespace

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
    m_squares[move.target] = m_squares[move.source];
    m_squares[move.source] = Piece::EMPTY;
    ++m_ply;
}

bool Board::is_terminal() const {
    const bool black_to_play = m_ply & 1;
    if (black_to_play) {
        return std::any_of(m_squares.begin(), m_squares.begin() + 8, is_white);
    } else {
        return std::any_of(m_squares.begin() + 55, m_squares.end(), is_black);
    }
}

}  // namespace breakthrough
