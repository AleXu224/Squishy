#include "formula/constant.hpp"
#include "../storeSetup.hpp"
#include "catch2/benchmark/catch_benchmark.hpp"
#include "formula/stat.hpp"
#include <catch2/catch_test_macros.hpp>


TEST_CASE("Constant", "[Bench]") {
	auto ctx = getContext();

	constexpr auto numberConstant = Formula::Constant(2.f);
	BENCHMARK("Constant") {
		return numberConstant.eval(ctx);
	};

	constexpr auto numberConstantFlat = Formula::ConstantFlat(2.f);
	BENCHMARK("ConstantFlat") {
		return numberConstantFlat.eval(ctx);
	};

	constexpr auto boolConstant = Formula::ConstantBool(true);
	BENCHMARK("ConstantBool") {
		return boolConstant.eval(ctx);
	};

	constexpr auto stat = Formula::Stat(Stat::hp);
	BENCHMARK("Stat") {
		return stat.eval(ctx);
	};

	using T = decltype(Stats::CharacterSheet::postMods);
	constexpr auto statPtr = Formula::StatPtr(&Stats::CharacterSheet::postMods, &T::hp);
	BENCHMARK("StatPtr") {
		return statPtr.eval(ctx);
	};
	
	using T = decltype(Stats::CharacterSheet::postMods);
	constexpr auto skillPtr = Formula::SkillPtr(&Stats::CharacterSheet::postMods, &T::normal, &T::_SkillValue::DMG);
	BENCHMARK("skillPtr") {
		return skillPtr.eval(ctx);
	};
}