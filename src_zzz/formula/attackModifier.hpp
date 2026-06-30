#pragma once

#include "formula/constant.hpp"
#include "formula/node.hpp"
#include "formula/operators.hpp"// IWYU pragma: keep

namespace Formula {
	using namespace Formula::Operators;
	struct EnemyModifierResistance {
		FloatNode physical = Formula::Constant{.value = 0.f};
		FloatNode fire = Formula::Constant{.value = 0.f};
		FloatNode ice = Formula::Constant{.value = 0.f};
		FloatNode electric = Formula::Constant{.value = 0.f};
		FloatNode ether = Formula::Constant{.value = 0.f};
		FloatNode wind = Formula::Constant{.value = 0.f};

		template<class T>
		EnemyModifierResistance operator+(const T &other) const {
			return EnemyModifierResistance{
				.physical = physical + other.physical,
				.fire = fire + other.fire,
				.ice = ice + other.ice,
				.electric = electric + other.electric,
				.ether = ether + other.ether,
				.wind = wind + other.wind,
			};
		}
	};

	struct EnemyModifier {
		FloatNode level = Formula::ConstantFlat{.value = 0.f};
		FloatNode stunMod = Formula::Constant{.value = 0.f};
		FloatNode baseDef = Formula::ConstantFlat{.value = 0.f};
		FloatNode DEFReduction = Formula::Constant{.value = 0.f};
		FloatNode DEFIgnored = Formula::Constant{.value = 0.f};
		FloatNode dazeRes = Formula::Constant{.value = 0.f};
		FloatNode daze = Formula::Constant{.value = 0.f};
		FloatNode dazeTaken = Formula::Constant{.value = 0.f};
		EnemyModifierResistance resistance{};

		template<class T>
		EnemyModifier operator+(const T &other) const {
			return EnemyModifier{
				.level = level + other.level,
				.stunMod = stunMod + other.stunMod,
				.baseDef = baseDef + other.baseDef,
				.DEFReduction = DEFReduction + other.DEFReduction,
				.DEFIgnored = DEFIgnored + other.DEFIgnored,
				.dazeRes = dazeRes + other.dazeRes,
				.daze = daze + other.daze,
				.dazeTaken = dazeTaken + other.dazeTaken,
				.resistance = resistance + other.resistance,
			};
		}
	};

	struct Modifier {
		Formula::FloatNode DMG{Formula::Constant({}, 0.f)};
		Formula::FloatNode directDMG{Formula::Constant({}, 0.f)};
		Formula::FloatNode additiveDMG{Formula::ConstantFlat({}, 0.f)};
		Formula::FloatNode additiveMultiplier{Formula::ConstantFlat({}, 0.f)};
		Formula::FloatNode multiplicativeDMG{Formula::Constant({}, 0.f)};
		Formula::FloatNode critRate{Formula::Constant({}, 0.f)};
		Formula::FloatNode critDMG{Formula::Constant({}, 0.f)};
		EnemyModifier enemy{};
	};
}// namespace Formula