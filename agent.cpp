#include "agent.h"
#include <stdexcept>

namespace breakthrough {

const std::vector<Move>& Agent::valid_moves(const Board& board) {
    m_valid_moves.clear();
    bool black_to_play = board.ply() & 1;

    if (board.is_terminal()) {
        return m_valid_moves;
    }

    for (Square s = 0; s < 64; ++s) {
        const Piece piece = board.at(s);
        if (black_to_play && is_black(piece)) {
            Square t = s - 8;
            if (is_empty(board.at(t))) {
                m_valid_moves.push_back({s, t});
            }
            if (s % 8 > 0 &&
                (is_white(board.at(t - 1)) || is_empty(board.at(t - 1)))) {
                m_valid_moves.push_back({s, t - 1});
            }
            if (s % 8 < 7 &&
                (is_white(board.at(t + 1)) || is_empty(board.at(t + 1)))) {
                m_valid_moves.push_back({s, t + 1});
            }
        } else if (!black_to_play && is_white(piece)) {
            Square t = s + 8;
            if (is_empty(board.at(t))) {
                m_valid_moves.push_back({s, t});
            }
            if (s % 8 > 0 &&
                (is_black(board.at(t - 1)) || is_empty(board.at(t - 1)))) {
                m_valid_moves.push_back({s, t - 1});
            }
            if (s % 8 < 7 &&
                (is_black(board.at(t + 1)) || is_empty(board.at(t + 1)))) {
                m_valid_moves.push_back({s, t + 1});
            }
        }
    }
    return m_valid_moves;
}

const Move Agent::random_move(const Board& board) {
    const auto& moves = valid_moves(board);
    if (moves.empty()) {
        throw std::runtime_error("No valid moves available");
    }
    std::uniform_int_distribution<> dist(0, moves.size() - 1);
    return moves[dist(eng)];
}

}  // namespace breakthrough
