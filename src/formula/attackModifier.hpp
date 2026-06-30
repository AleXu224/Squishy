#pragma once

#include "formula/constant.hpp"
#include "formula/node.hpp"
#include "formula/operators.hpp"// IWYU pragma: keep

namespace Formula {
	using namespace Formula::Operators;
	struct EnemyModifierResistance {
		FloatNode pyro = Formula::Constant{.value = 0.f};
		FloatNode hydro = Formula::Constant{.value = 0.f};
		FloatNode cryo = Formula::Constant{.value = 0.f};
		FloatNode electro = Formula::Constant{.value = 0.f};
		FloatNode dendro = Formula::Constant{.value = 0.f};
		FloatNode anemo = Formula::Constant{.value = 0.f};
		FloatNode geo = Formula::Constant{.value = 0.f};
		FloatNode physical = Formula::Constant{.value = 0.f};

		template<class T>
		EnemyModifierResistance operator+(const T &other) const {
			return EnemyModifierResistance{
				.pyro = pyro + other.pyro,
				.hydro = hydro + other.hydro,
				.cryo = cryo + other.cryo,
				.electro = electro + other.electro,
				.dendro = dendro + other.dendro,
				.anemo = anemo + other.anemo,
				.geo = geo + other.geo,
				.physical = physical + other.physical,
			};
		}
	};

	struct EnemyModifier {
		FloatNode level = Formula::ConstantFlat{.value = 0.f};
		FloatNode DEFReduction = Formula::Constant{.value = 0.f};
		FloatNode DEFIgnored = Formula::Constant{.value = 0.f};
		EnemyModifierResistance resistance{};

		template<class T>
		EnemyModifier operator+(const T &other) const {
			return EnemyModifier{
				.level = level + other.level,
				.DEFReduction = DEFReduction + other.DEFReduction,
				.DEFIgnored = DEFIgnored + other.DEFIgnored,
				.resistance = resistance + other.resistance,
			};
		}
	};

	struct Modifier {
		Formula::FloatNode DMG{Formula::Constant({}, 0.f)};
		Formula::FloatNode additiveDMG{Formula::ConstantFlat({}, 0.f)};
		Formula::FloatNode multiplicativeDMG{Formula::Constant({}, 0.f)};
		Formula::FloatNode elevation{Formula::Constant({}, 0.f)};
		Formula::FloatNode critRate{Formula::Constant({}, 0.f)};
		Formula::FloatNode critDMG{Formula::Constant({}, 0.f)};
		EnemyModifier enemy{};
	};
}// namespace Formula