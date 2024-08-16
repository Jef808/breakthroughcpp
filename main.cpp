#include <iostream>
#include "board.h"

using namespace breakthrough;

auto main(int argc, char *argv[]) -> int {
    Board board{};

    std::cout << board.fen() << std::endl;

    return 0;
}
