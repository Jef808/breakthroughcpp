#include "catch2/catch_test_macros.hpp"
#include "board.h"
#include "movegen.h"
#include <sstream>

using namespace breakthrough;

// Helper function to print Square
inline std::string print(Square s) {
    const char files[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    std::stringstream result;
    result << files[s % 8];
    result << int(s / 8) + 1;
    return result.str();
}

// Operator overload for Move printing
std::ostream& operator<<(std::ostream& out, const Move& move) {
    return out << print(move.source) << print(move.target);
}

TEST_CASE("Piece functions", "[board]") {
    SECTION("is_empty function") {
        REQUIRE(is_empty(Piece::EMPTY) == true);
        REQUIRE(is_empty(Piece::WHITE) == false);
        REQUIRE(is_empty(Piece::BLACK) == false);
    }

    SECTION("is_white function") {
        REQUIRE(is_white(Piece::EMPTY) == false);
        REQUIRE(is_white(Piece::WHITE) == true);
        REQUIRE(is_white(Piece::BLACK) == false);
    }

    SECTION("is_black function") {
        REQUIRE(is_black(Piece::EMPTY) == false);
        REQUIRE(is_black(Piece::WHITE) == false);
        REQUIRE(is_black(Piece::BLACK) == true);
    }
}

TEST_CASE("Board default constructor initializes pieces correctly", "[board]") {
    Board board;

    SECTION("First 16 squares are white pieces") {
        for (int i = 0; i < 16; ++i) {
            REQUIRE(board.at(Square(i)) == Piece::WHITE);
        }
    }
    SECTION("Middle 32 squares are empty") {
        for (int i = 16; i < 48; ++i) {
            REQUIRE(board.at(Square(i)) == Piece::EMPTY);
        }
    }
    SECTION("Last 16 squares are black pieces") {
        for (int i = 48; i < 64; ++i) {
            REQUIRE(board.at(Square(i)) == Piece::BLACK);
        }
    }
}

TEST_CASE("Board outputs correct fen strings", "[board]") {
    Board board;

    SECTION("Initial game has correct fen string") {
        REQUIRE(board.fen() == "PPPPPPPP/PPPPPPPP/////pppppppp/pppppppp w - - 0 1");
    }
}

TEST_CASE("MoveGen functionality", "[movegen]") {
    Board board{};
    MoveGen movegen;

    SECTION("Initial board state") {
        REQUIRE_FALSE(board.is_terminal());

        auto moves = movegen.valid_moves(board);
        REQUIRE(moves.size() > 0);
        // Add more specific checks for the initial moves
    }

    SECTION("After first move") {
        Move move{11, 20};
        board.play(move);

        REQUIRE_FALSE(board.is_terminal());

        auto moves = movegen.valid_moves(board);
        REQUIRE(moves.size() > 0);
        // Add more specific checks for moves after the first move
    }

    SECTION("After second move") {
        Move move1{11, 20};
        Move move2{48, 40};
        board.play(move1);
        board.play(move2);

        REQUIRE_FALSE(board.is_terminal());

        auto moves = movegen.valid_moves(board);
        REQUIRE(moves.size() > 0);
        // Add more specific checks for moves after the second move
    }
}

TEST_CASE("Move printing", "[move]") {
    Move move1{11, 20};
    Move move2{48, 40};
    std::stringstream ss1;
    std::stringstream ss2;
    ss1 << move1;
    ss2 << move2;
    REQUIRE(ss1.str() == "d2e3");
    REQUIRE(ss2.str() == "a7a6");
}
