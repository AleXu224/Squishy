#include "formula/constant.hpp"
#include "formula/operators.hpp"
#include "storeSetup.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Boolean", "[Equality]") {
	constexpr auto falseConstant = Formula::ConstantBool(false);
	constexpr auto trueConstant = Formula::ConstantBool(true);
	constexpr auto someConstant1 = Formula::Constant(2.f);
	constexpr auto someConstant2 = Formula::ConstantFlat(3.f);

	REQUIRE(eval(falseConstant == falseConstant) == true);
	REQUIRE(eval(falseConstant == trueConstant) == false);
	REQUIRE(eval(falseConstant == false) == true);
	REQUIRE(eval(false == falseConstant) == true);

	REQUIRE(eval(someConstant1 == someConstant1) == true);
	REQUIRE(eval(someConstant1 == 2.f) == true);
	REQUIRE(eval(2.f == someConstant1) == true);
	REQUIRE(eval(someConstant2 == someConstant2) == true);
	REQUIRE(eval(someConstant2 == 3.f) == true);
	REQUIRE(eval(3.f == someConstant2) == true);
}

TEST_CASE("Boolean", "[Inequality]") {
	constexpr auto falseConstant = Formula::ConstantBool(false);
	constexpr auto trueConstant = Formula::ConstantBool(true);
	constexpr auto someConstant1 = Formula::Constant(2.f);
	constexpr auto someConstant2 = Formula::ConstantFlat(3.f);

	REQUIRE(eval(falseConstant != falseConstant) == false);
	REQUIRE(eval(falseConstant != trueConstant) == true);
	REQUIRE(eval(falseConstant != false) == false);
	REQUIRE(eval(false != falseConstant) == false);

	REQUIRE(eval(someConstant1 != someConstant1) == false);
	REQUIRE(eval(someConstant1 != 2.f) == false);
	REQUIRE(eval(2.f != someConstant1) == false);
	REQUIRE(eval(someConstant2 != someConstant2) == false);
	REQUIRE(eval(someConstant2 != 3.f) == false);
	REQUIRE(eval(3.f != someConstant2) == false);
}
TEST_CASE("Boolean", "[Not]") {
	constexpr auto falseConstant = Formula::ConstantBool(false);
	constexpr auto trueConstant = Formula::ConstantBool(true);

	REQUIRE(eval(!falseConstant) == true);
	REQUIRE(eval(!trueConstant) == false);
}
TEST_CASE("Boolean", "[Less]") {
	constexpr auto someConstant1 = Formula::Constant(2.f);
	constexpr auto someConstant2 = Formula::ConstantFlat(3.f);
	constexpr auto someConstant3 = Formula::ConstantFlat(3.f);

	REQUIRE(eval(someConstant1 < someConstant2) == true);
	REQUIRE(eval(someConstant2 < someConstant1) == false);
	REQUIRE(eval(someConstant2 < someConstant3) == false);
	REQUIRE(eval(someConstant1 < 1.f) == false);
	REQUIRE(eval(someConstant1 < 2.f) == false);
	REQUIRE(eval(someConstant1 < 3.f) == true);
	REQUIRE(eval(1.f < someConstant1) == true);
	REQUIRE(eval(2.f < someConstant1) == false);
	REQUIRE(eval(3.f < someConstant1) == false);
}
TEST_CASE("Boolean", "[LessEqual]") {
	constexpr auto someConstant1 = Formula::Constant(2.f);
	constexpr auto someConstant2 = Formula::ConstantFlat(3.f);
	constexpr auto someConstant3 = Formula::ConstantFlat(3.f);

	REQUIRE(eval(someConstant1 <= someConstant2) == true);
	REQUIRE(eval(someConstant2 <= someConstant1) == false);
	REQUIRE(eval(someConstant2 <= someConstant3) == true);
	REQUIRE(eval(someConstant1 <= 1.f) == false);
	REQUIRE(eval(someConstant1 <= 2.f) == true);
	REQUIRE(eval(someConstant1 <= 3.f) == true);
	REQUIRE(eval(1.f <= someConstant1) == true);
	REQUIRE(eval(2.f <= someConstant1) == true);
	REQUIRE(eval(3.f <= someConstant1) == false);
}
TEST_CASE("Boolean", "[Greater]") {
	constexpr auto someConstant1 = Formula::Constant(2.f);
	constexpr auto someConstant2 = Formula::ConstantFlat(3.f);
	constexpr auto someConstant3 = Formula::ConstantFlat(3.f);

	REQUIRE(eval(someConstant1 > someConstant2) == false);
	REQUIRE(eval(someConstant2 > someConstant1) == true);
	REQUIRE(eval(someConstant2 > someConstant3) == false);
	REQUIRE(eval(someConstant1 > 1.f) == true);
	REQUIRE(eval(someConstant1 > 2.f) == false);
	REQUIRE(eval(someConstant1 > 3.f) == false);
	REQUIRE(eval(1.f > someConstant1) == false);
	REQUIRE(eval(2.f > someConstant1) == false);
	REQUIRE(eval(3.f > someConstant1) == true);
}
TEST_CASE("Boolean", "[GreaterEqual]") {
	constexpr auto someConstant1 = Formula::Constant(2.f);
	constexpr auto someConstant2 = Formula::ConstantFlat(3.f);
	constexpr auto someConstant3 = Formula::ConstantFlat(3.f);

	REQUIRE(eval(someConstant1 >= someConstant2) == false);
	REQUIRE(eval(someConstant2 >= someConstant1) == true);
	REQUIRE(eval(someConstant2 >= someConstant3) == true);
	REQUIRE(eval(someConstant1 >= 1.f) == true);
	REQUIRE(eval(someConstant1 >= 2.f) == true);
	REQUIRE(eval(someConstant1 >= 3.f) == false);
	REQUIRE(eval(1.f >= someConstant1) == false);
	REQUIRE(eval(2.f >= someConstant1) == true);
	REQUIRE(eval(3.f >= someConstant1) == true);
}

TEST_CASE("Boolean", "[And]") {
	constexpr auto falseConstant = Formula::ConstantBool(false);
	constexpr auto trueConstant = Formula::ConstantBool(true);

	REQUIRE(eval(falseConstant && falseConstant) == false);
	REQUIRE(eval(falseConstant && trueConstant) == false);
	REQUIRE(eval(trueConstant && falseConstant) == false);
	REQUIRE(eval(trueConstant && trueConstant) == true);

	REQUIRE(eval(trueConstant && true) == true);
	REQUIRE(eval(true && trueConstant) == true);
	REQUIRE(eval(trueConstant && false) == false);
	REQUIRE(eval(false && trueConstant) == false);

	REQUIRE(eval(falseConstant && true) == false);
	REQUIRE(eval(true && falseConstant) == false);
	REQUIRE(eval(falseConstant && false) == false);
	REQUIRE(eval(false && falseConstant) == false);
}
TEST_CASE("Boolean", "[Or]") {
	constexpr auto falseConstant = Formula::ConstantBool(false);
	constexpr auto trueConstant = Formula::ConstantBool(true);

	REQUIRE(eval(falseConstant || falseConstant) == false);
	REQUIRE(eval(falseConstant || trueConstant) == true);
	REQUIRE(eval(trueConstant || falseConstant) == true);
	REQUIRE(eval(trueConstant || trueConstant) == true);

	REQUIRE(eval(trueConstant || true) == true);
	REQUIRE(eval(true || trueConstant) == true);
	REQUIRE(eval(trueConstant || false) == true);
	REQUIRE(eval(false || trueConstant) == true);

	REQUIRE(eval(falseConstant || true) == true);
	REQUIRE(eval(true || falseConstant) == true);
	REQUIRE(eval(falseConstant || false) == false);
	REQUIRE(eval(false || falseConstant) == false);
}
