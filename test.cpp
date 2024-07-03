#include "catch2/catch_test_macros.hpp"
#include "board.h"

TEST_CASE("Board piece type checks", "[board]") {
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

TEST_CASE("Board default constructor initializes pieces correctly", "[Board]") {
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
