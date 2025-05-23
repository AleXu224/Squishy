#pragma once

#include "formula/multiplier.hpp"
#include "formula/operators.hpp"
#include "formula/reactionStat.hpp"
#include "misc/element.hpp"
#include "string_view"


namespace Reaction {
	using namespace Formula::Operators;
	[[nodiscard]] static Formula::FloatNode makeAdditiveFormula(float multiplier, Misc::Reaction reaction) {
		constexpr auto levelMultiplier = Formula::LevelMultiplier{};
		auto emBonus = (5.f * Modifiers::total().em) / (Modifiers::total().em + 1200.f);
		auto reactionBonus = Formula::ReactionStat<Misc::SkillStat::DMG>(reaction);
		return Formula::Constant(multiplier) * levelMultiplier * (1.f + emBonus + reactionBonus);
	}
	struct Additive {
		std::string_view name;
		float multiplier{};
		Misc::Element trigger{};
		Misc::Reaction reaction{};
		Misc::NodeReaction nodeReaction{};
		Formula::FloatNode formula = makeAdditiveFormula(multiplier, reaction);
	};

	namespace List {
		struct Additive {
			const static inline ::Reaction::Additive aggravate = Reaction::Additive{
				.name = "Aggravate",
				.multiplier = 1.15f,
				.trigger = Misc::Element::electro,
				.reaction = Misc::Reaction::aggravate,
				.nodeReaction = Misc::NodeReaction::aggravate,
			};
			const static inline ::Reaction::Additive spread = Reaction::Additive{
				.name = "Spread",
				.multiplier = 1.25f,
				.trigger = Misc::Element::dendro,
				.reaction = Misc::Reaction::spread,
				.nodeReaction = Misc::NodeReaction::spread,
			};
		};
	}// namespace List
}// namespace Reaction