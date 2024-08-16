#include <iostream>
#include "board.h"

auto main(int argc, char *argv[]) -> int {
    Board board{};

    std::cout << board.fen() << std::endl;

    return 0;
}
