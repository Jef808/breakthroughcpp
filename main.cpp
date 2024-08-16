#include "board.h"
#include "agent.h"
#include <iostream>
#include <sstream>

using namespace breakthrough;

inline std::string print(Square s) {
    const char files[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    std::stringstream result;
    result << files[s % 8];
    result << int(s / 8) + 1;
    return result.str();
}

std::ostream& operator<<(std::ostream& out, const Move& move) {
    return out << print(move.source) << print(move.target);
}

auto main() -> int {
    Board board{};
    Agent agent;

    const auto& valid_moves = agent.valid_moves(board);

    for (auto move : valid_moves) {
        std::cout << move << '\n';
    }
    std::cout << std::endl;

    return 0;
}
