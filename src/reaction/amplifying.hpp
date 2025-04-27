#pragma once

#include "formula/operators.hpp"
#include "formula/reactionStat.hpp"
#include "misc/element.hpp"
#include "modifiers/total/total.hpp"
#include "string_view"


namespace Reaction {
	using namespace Formula::Operators;
	[[nodiscard]] static Formula::FloatNode makeAmplifyingFormula(float multiplier, Misc::Reaction reaction) {
		auto emBonus = 2.78f * (Modifiers::total().em / (Modifiers::total().em + 1400.f));
		auto reactionBonus = Formula::ReactionStat<Misc::SkillStat::DMG>(reaction);
		return Formula::Constant(multiplier) * (1 + emBonus + reactionBonus);
	}
	struct Amplifying {
		std::string_view name;
		float multiplier{};
		// The final element applied that will trigger this reaction
		Misc::Element trigger{};
		Misc::Reaction reaction{};
		Misc::NodeReaction nodeReaction{};
		Formula::FloatNode formula = makeAmplifyingFormula(multiplier, reaction);
	};

	namespace List {
		struct Amplifying {
			const static inline ::Reaction::Amplifying forwardVape = Reaction::Amplifying{
				.name = "Forward Vape",
				.multiplier = 2.f,
				.trigger = Misc::Element::hydro,
				.reaction = Misc::Reaction::vape,
				.nodeReaction = Misc::NodeReaction::forwardVape,
			};
			const static inline ::Reaction::Amplifying reverseVape = Reaction::Amplifying{
				.name = "Reverse Vape",
				.multiplier = 1.5f,
				.trigger = Misc::Element::pyro,
				.reaction = Misc::Reaction::vape,
				.nodeReaction = Misc::NodeReaction::reverseVape,
			};
			const static inline ::Reaction::Amplifying forwardMelt = Reaction::Amplifying{
				.name = "Forward Melt",
				.multiplier = 2.f,
				.trigger = Misc::Element::pyro,
				.reaction = Misc::Reaction::melt,
				.nodeReaction = Misc::NodeReaction::forwardMelt,
			};
			const static inline ::Reaction::Amplifying reverseMelt = Reaction::Amplifying{
				.name = "Reverse Melt",
				.multiplier = 1.5f,
				.trigger = Misc::Element::cryo,
				.reaction = Misc::Reaction::melt,
				.nodeReaction = Misc::NodeReaction::reverseMelt,
			};
		};
	}// namespace List
}// namespace Reaction