#include "formula/formula.hpp"
#include "storeSetup.hpp"
#include <catch2/catch_test_macros.hpp>


TEST_CASE("Boolean") {
	const auto &[team, character] = ::getCharacter();

	constexpr auto falseConstant = Formula::ConstantBool(false);
	constexpr auto trueConstant = Formula::ConstantBool(true);
	constexpr auto someConstant1 = Formula::Constant(2.f);
	constexpr auto someConstant2 = Formula::ConstantFlat(3.f);
	constexpr auto someConstant3 = Formula::ConstantFlat(3.f);

	// Equality
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

	// Inequality
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

	// Not
	REQUIRE(eval(!falseConstant) == true);

	// Less
	REQUIRE(eval(someConstant1 < someConstant2) == true);
	REQUIRE(eval(someConstant2 < someConstant1) == false);
	REQUIRE(eval(someConstant2 < someConstant3) == false);
	REQUIRE(eval(someConstant1 < 1.f) == false);
	REQUIRE(eval(someConstant1 < 2.f) == false);
	REQUIRE(eval(someConstant1 < 3.f) == true);
	REQUIRE(eval(1.f < someConstant1) == true);
	REQUIRE(eval(2.f < someConstant1) == false);
	REQUIRE(eval(3.f < someConstant1) == false);

	// Less Equal
	REQUIRE(eval(someConstant1 <= someConstant2) == true);
	REQUIRE(eval(someConstant2 <= someConstant1) == false);
	REQUIRE(eval(someConstant2 <= someConstant3) == true);
	REQUIRE(eval(someConstant1 <= 1.f) == false);
	REQUIRE(eval(someConstant1 <= 2.f) == true);
	REQUIRE(eval(someConstant1 <= 3.f) == true);
	REQUIRE(eval(1.f <= someConstant1) == true);
	REQUIRE(eval(2.f <= someConstant1) == true);
	REQUIRE(eval(3.f <= someConstant1) == false);

	// Greater
	REQUIRE(eval(someConstant1 > someConstant2) == false);
	REQUIRE(eval(someConstant2 > someConstant1) == true);
	REQUIRE(eval(someConstant2 > someConstant3) == false);
	REQUIRE(eval(someConstant1 > 1.f) == true);
	REQUIRE(eval(someConstant1 > 2.f) == false);
	REQUIRE(eval(someConstant1 > 3.f) == false);
	REQUIRE(eval(1.f > someConstant1) == false);
	REQUIRE(eval(2.f > someConstant1) == false);
	REQUIRE(eval(3.f > someConstant1) == true);

	// Greater Equal
	REQUIRE(eval(someConstant1 >= someConstant2) == false);
	REQUIRE(eval(someConstant2 >= someConstant1) == true);
	REQUIRE(eval(someConstant2 >= someConstant3) == true);
	REQUIRE(eval(someConstant1 >= 1.f) == true);
	REQUIRE(eval(someConstant1 >= 2.f) == true);
	REQUIRE(eval(someConstant1 >= 3.f) == false);
	REQUIRE(eval(1.f >= someConstant1) == false);
	REQUIRE(eval(2.f >= someConstant1) == true);
	REQUIRE(eval(3.f >= someConstant1) == true);

	// And
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

	// Or
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