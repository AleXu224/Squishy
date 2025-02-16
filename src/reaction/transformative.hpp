#pragma once

#include "array"
#include "formula/clamp.hpp"
#include "formula/enemy.hpp"
#include "formula/multiplier.hpp"
#include "formula/operators.hpp"
#include "misc/element.hpp"
#include "string_view"


namespace Reaction {
	using namespace Formula::Operators;
	template<auto reaction>
	[[nodiscard]] static constexpr auto makeTransformativeFormula(float multiplier, Misc::Element element) {
		constexpr auto levelMultiplier = Formula::LevelMultiplier{};
		constexpr auto emBonus = (Formula::ConstantFlat(16.f) * Modifiers::total.em) / (Modifiers::total.em + Formula::ConstantFlat(2000.f));
		auto reactionBonus = reaction.DMG;
		auto resMultiplier = Formula::EnemyResMultiplier({}, element);
		auto critMultiplier = 1.f + Formula::Clamp(reaction.critRate, 0.f, 1.f) * reaction.critDMG;
		return Formula::Constant(multiplier) * levelMultiplier * (1.f + emBonus + reactionBonus) * resMultiplier * critMultiplier;
	}
	template<auto ReactionFrm, class Trigs>
	struct Transformative {
		std::string_view name;
		float multiplier{};
		Misc::Element damageElement{};
		Trigs triggers;
		decltype(makeTransformativeFormula<ReactionFrm>(std::declval<float>(), std::declval<Misc::Element>())) formula = makeTransformativeFormula<ReactionFrm>(multiplier, damageElement);
	};

	namespace List {
		struct Transformative {
			static constexpr auto burning = Reaction::Transformative<Modifiers::total.burning, std::array<Misc::Element, 1>>{
				.name = "Burning",
				.multiplier = 0.25f,
				.damageElement = Misc::Element::pyro,
				.triggers = std::array{
					Misc::Element::pyro,
				},
			};
			static constexpr auto superconduct = Reaction::Transformative<Modifiers::total.superconduct, std::array<Misc::Element, 2>>{
				.name = "Superconduct",
				.multiplier = 1.5f,
				.damageElement = Misc::Element::cryo,
				.triggers = std::array{
					Misc::Element::cryo,
					Misc::Element::electro,
				},
			};
			static constexpr auto pyroSwirl = Reaction::Transformative<Modifiers::total.swirl, std::array<Misc::Element, 1>>{
				.name = "Pyro Swirl",
				.multiplier = 0.6f,
				.damageElement = Misc::Element::pyro,
				.triggers = std::array{
					Misc::Element::anemo,
				},
			};
			static constexpr auto hydroSwirl = Reaction::Transformative<Modifiers::total.swirl, std::array<Misc::Element, 1>>{
				.name = "Hydro Swirl",
				.multiplier = 0.6f,
				.damageElement = Misc::Element::hydro,
				.triggers = std::array{
					Misc::Element::anemo,
				},
			};
			static constexpr auto electroSwirl = Reaction::Transformative<Modifiers::total.swirl, std::array<Misc::Element, 1>>{
				.name = "Electro Swirl",
				.multiplier = 0.6f,
				.damageElement = Misc::Element::electro,
				.triggers = std::array{
					Misc::Element::anemo,
				},
			};
			static constexpr auto cryoSwirl = Reaction::Transformative<Modifiers::total.swirl, std::array<Misc::Element, 1>>{
				.name = "Cryo Swirl",
				.multiplier = 0.6f,
				.damageElement = Misc::Element::cryo,
				.triggers = std::array{
					Misc::Element::anemo,
				},
			};
			static constexpr auto electroCharged = Reaction::Transformative<Modifiers::total.electroCharged, std::array<Misc::Element, 2>>{
				.name = "Electro-Charged",
				.multiplier = 2.f,
				.damageElement = Misc::Element::electro,
				.triggers = std::array{
					Misc::Element::electro,
					Misc::Element::hydro,
				},
			};
			static constexpr auto shattered = Reaction::Transformative<Modifiers::total.shattered, std::array<Misc::Element, 8>>{
				.name = "Shattered",
				.multiplier = 3.f,
				.damageElement = Misc::Element::physical,
				.triggers = std::array{
					Misc::Element::pyro,
					Misc::Element::hydro,
					Misc::Element::cryo,
					Misc::Element::electro,
					Misc::Element::dendro,
					Misc::Element::anemo,
					Misc::Element::geo,
					Misc::Element::physical,
				},
			};
			static constexpr auto overloaded = Reaction::Transformative<Modifiers::total.overloaded, std::array<Misc::Element, 2>>{
				.name = "Overloaded",
				.multiplier = 2.75f,
				.damageElement = Misc::Element::pyro,
				.triggers = std::array{
					Misc::Element::pyro,
					Misc::Element::electro,
				},
			};
			static constexpr auto bloom = Reaction::Transformative<Modifiers::total.bloom, std::array<Misc::Element, 2>>{
				.name = "Bloom",
				.multiplier = 2.0f,
				.damageElement = Misc::Element::dendro,
				.triggers = std::array{
					Misc::Element::hydro,
					Misc::Element::dendro,
				},
			};
			static constexpr auto burgeon = Reaction::Transformative<Modifiers::total.burgeon, std::array<Misc::Element, 1>>{
				.name = "Burgeon",
				.multiplier = 3.0f,
				.damageElement = Misc::Element::dendro,
				.triggers = std::array{
					Misc::Element::pyro,
				},
			};
			static constexpr auto hyperbloom = Reaction::Transformative<Modifiers::total.hyperbloom, std::array<Misc::Element, 1>>{
				.name = "Hyperbloom",
				.multiplier = 3.0f,
				.damageElement = Misc::Element::dendro,
				.triggers = std::array{
					Misc::Element::electro,
				},
			};
		};
	}// namespace List
}// namespace Reaction