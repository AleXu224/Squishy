#include "formula/constant.hpp"
#include "storeSetup.hpp"
#include <catch2/catch_test_macros.hpp>


TEST_CASE("Constant") {
	constexpr auto falseConstant = Formula::ConstantBool(false);
	constexpr auto trueConstant = Formula::ConstantBool(true);

	constexpr auto numberConstant1 = Formula::Constant(2.f);
	constexpr auto numberConstant2 = Formula::ConstantFlat(2.f);

	REQUIRE(eval(falseConstant) == false);
	REQUIRE(eval(trueConstant) == true);
	REQUIRE(eval(numberConstant1) == 2.f);
	REQUIRE(eval(numberConstant2) == 2.f);
}