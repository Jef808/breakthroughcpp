#include "board.h"

#include <vector>

namespace breakthrough {

class MoveGen {
public:
    MoveGen() = default;
    const std::vector<Move>& valid_moves(const Board& board);

private:
    std::vector<Move> m_valid_moves;
};

} // namespace breakthrough
