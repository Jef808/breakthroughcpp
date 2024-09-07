#ifndef MCTS_H_
#define MCTS_H_

#include "board.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace breakthrough {

struct Node {
    Board state;
    int wins{0};
    int visits{0};
    Move move{-1, -1};
    uint64_t parent{0};
    std::vector<uint64_t> children;
};

class MCTS {
public:
    MCTS() = default;
    void ponder_mcts(const Board& board, int ms);
    Move choose_best(const Board& board);

private:
    std::unordered_map<uint64_t, Node> m_stats;
};

} // namespace breakthrough

#endif // MCTS_H_
