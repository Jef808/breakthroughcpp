#include <iostream>
#include "board.h"

auto main(int argc, char *argv[]) -> int {

    // Should return 1, 0, 0 respectively
    std::cout << "is_empty(EMPTY)" << std::endl;
    std::cout << is_empty(EMPTY) << std::endl;
    std::cout << "is_emptyWHITE" << std::endl;
    std::cout << is_empty(WHITE) << std::endl;
    std::cout << "is_emptyBLACK" << std::endl;
    std::cout << is_empty(BLACK) << std::endl;

    // Should return 0, 1, 0 respectively
    std::cout << "is_white(EMPTY)" << std::endl;
    std::cout << is_white(EMPTY) << std::endl;
    std::cout << "is_whiteWHITE" << std::endl;
    std::cout << is_white(WHITE) << std::endl;
    std::cout << "is_whiteBLACK" << std::endl;
    std::cout << is_white(BLACK) << std::endl;

    // Should return 0, 0, 1 respectively
    std::cout << "is_black(EMPTY)" << std::endl;
    std::cout << is_black(EMPTY) << std::endl;
    std::cout << "is_blackWHITE" << std::endl;
    std::cout << is_black(WHITE) << std::endl;
    std::cout << "is_blackBLACK" << std::endl;
    std::cout << is_black(BLACK) << std::endl;

return 0;
}
