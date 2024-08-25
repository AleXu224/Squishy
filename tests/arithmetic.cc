#include "storeSetup.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Arithmetic") {
    constexpr auto numberConstant1 = Formula::Constant(2.f);
    constexpr auto numberConstant2 = Formula::Constant(3.f);

    // Addition
    REQUIRE(eval(numberConstant1 + numberConstant2) == 5.f);
    REQUIRE(eval(numberConstant2 + numberConstant1) == 5.f);
    REQUIRE(eval(numberConstant1 + 5.f) == 7.f);
    REQUIRE(eval(numberConstant2 + 5.f) == 8.f);
    REQUIRE(eval(5.f + numberConstant1) == 7.f);
    REQUIRE(eval(5.f + numberConstant2) == 8.f);

    // Product
	REQUIRE(eval(numberConstant1 * numberConstant2) == 6.f);
	REQUIRE(eval(numberConstant2 * numberConstant1) == 6.f);
	REQUIRE(eval(numberConstant1 * 5.f) == 10.f);
	REQUIRE(eval(numberConstant2 * 5.f) == 15.f);
	REQUIRE(eval(5.f * numberConstant1) == 10.f);
	REQUIRE(eval(5.f * numberConstant2) == 15.f);
}