#include "board.h"
#include "mcts.h"
#include "movegen.h"
#include <iostream>
#include <optional>
#include <charconv>
#include <random>
#include <sstream>

using namespace breakthrough;

constexpr auto to_int = [](std::string_view sv) -> std::optional<int> {
    int value{};
    if (std::from_chars(sv.data(), sv.data() + sv.size(), value).ec == std::errc{}) {
        return value;
    }
    return std::nullopt;
};

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

inline Square stos(std::string_view s) {
    char file = s[0];
    int ifile;
    switch (file) {
        case 'a': ifile = 0; break;
        case 'b': ifile = 1; break;
        case 'c': ifile = 2; break;
        case 'd': ifile = 3; break;
        case 'e': ifile = 4; break;
        case 'f': ifile = 5; break;
        case 'g': ifile = 6; break;
        case 'h': ifile = 7; break;
        default: throw std::runtime_error("Invalid file");
    }
    s.remove_prefix(1);
    int irow = *to_int(s);

    return ifile + (irow - 1) * 8;
}

inline Move get_move(std::string_view s) {
    Move move;
    move.source = stos(s.substr(0, 2));
    move.target = stos(s.substr(2, 2));
    return move;
};

std::optional<Move> turn_input() {
    static std::string buffer;
    Move move{-1, -1};
    std::getline(std::cin, buffer);
    if (buffer[0] != 'N') {
        move = get_move(buffer);
    }
    int n;
    std::cin >> n; std::cin.ignore();
    for (auto i = 0; i < n; ++i) {
        std::getline(std::cin, buffer);
    }
    return move.source == -1 ? std::nullopt : std::make_optional(move);
}

int main() {
    Board board;
    MCTS mcts;

    Move move;
    std::string buffer;

    while (true) {
        auto move_in = turn_input();
        if (move_in) {
            board.play(*move_in);
        }

        mcts.ponder(board, 90);
        move = mcts.choose_best(board);
        board.play(move);

        std::cout << move << std::endl;
    }

    return 0;
}
