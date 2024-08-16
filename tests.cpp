#include "catch2/catch_test_macros.hpp"
#include "board.h"

TEST_CASE("Board piece type checks", "[board]") {
    SECTION("is_empty function") {
        REQUIRE(is_empty(EMPTY) == true);
        REQUIRE(is_empty(WHITE) == false);
        REQUIRE(is_empty(BLACK) == false);
    }

    SECTION("is_white function") {
        REQUIRE(is_white(EMPTY) == false);
        REQUIRE(is_white(WHITE) == true);
        REQUIRE(is_white(BLACK) == false);
    }

    SECTION("is_black function") {
        REQUIRE(is_black(EMPTY) == false);
        REQUIRE(is_black(WHITE) == false);
        REQUIRE(is_black(BLACK) == true);
    }
}
