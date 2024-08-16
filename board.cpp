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
    std::string fen_string;
    fen_string.reserve(90);

    auto fen = std::back_inserter(fen_string);
    auto square = m_squares.begin();

    for (int rank = 0; rank < 8; ++rank) {
        auto end = square + 8;
        auto empty_count = 0;

        for (; square != end; ++square) {
            if (*square == Piece::WHITE) {
                if (empty_count) {
                    fen = empty_count;
                }
                fen = 'P';
                empty_count = 0;
            }
            else if (*square == Piece::BLACK) {
                if (empty_count) {
                    fen = empty_count;
                }
                fen = 'p';
                empty_count = 0;
            }
            else {
                ++empty_count;
            }
        }
        if (square != m_squares.end()) {
            fen = '/';
        }
    }
    fen = ' ';

    auto black_to_play = m_ply & 1;
    fen = black_to_play ? 'b' : 'w';

    auto constexpr flags = " - - 0 ";
    std::copy_n(&flags[0], 7, fen);

    int full_moves = m_ply / 2 + 1;
    fen = full_moves;

    return fen_string;
}

void Board::play(Move move) {
    m_squares[move.target] = m_squares[move.source];
    m_squares[move.source] = Piece::EMPTY;
}

}  // namespace breakthrough
