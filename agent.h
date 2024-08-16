#ifndef AGENT_H_
#define AGENT_H_

#include "board.h"
#include <random>
#include <vector>

namespace breakthrough {

class Agent {
public:
    Agent() = default;

    const std::vector<Move>& valid_moves(const Board& board);
    const Move random_move(const Board& board);

private:
    std::vector<Move> m_valid_moves{};
    std::random_device rd;
    std::mt19937 eng{rd()};
};

}  // namespace breakthrough

#endif // AGENT_H_
