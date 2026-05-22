#pragma once

#include "formula/constant.hpp"
#include "formula/node.hpp"

namespace Formula {
	struct EnemyModifierResistance {
		FloatNode physical = Formula::Constant{.value = 0.f};
		FloatNode fire = Formula::Constant{.value = 0.f};
		FloatNode ice = Formula::Constant{.value = 0.f};
		FloatNode electric = Formula::Constant{.value = 0.f};
		FloatNode ether = Formula::Constant{.value = 0.f};
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
	};

	struct Modifier {
		Formula::FloatNode DMG{Formula::Constant({}, 0.f)};
		Formula::FloatNode additiveDMG{Formula::ConstantFlat({}, 0.f)};
		Formula::FloatNode multiplicativeDMG{Formula::Constant({}, 0.f)};
		Formula::FloatNode critRate{Formula::Constant({}, 0.f)};
		Formula::FloatNode critDMG{Formula::Constant({}, 0.f)};
		EnemyModifier enemy{};
	};
}// namespace Formula