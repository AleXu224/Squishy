#pragma once

#include "formula/operators.hpp"
#include "formula/reactionStat.hpp"
#include "misc/element.hpp"
#include "modifiers/total/total.hpp"
#include "string_view"


namespace Reaction {
	using namespace Formula::Operators;
	[[nodiscard]] static constexpr auto makeAmplifyingFormula(float multiplier, Misc::Reaction reaction) {
		constexpr auto emBonus = 2.78f * (Modifiers::total.em / (Modifiers::total.em + 1400.f));
		auto reactionBonus = Formula::ReactionStat<Misc::SkillStat::DMG>(reaction);
		return Formula::Constant(multiplier) * (1 + emBonus + reactionBonus);
	}
	struct Amplifying {
		std::string_view name;
		float multiplier{};
		// The final element applied that will trigger this reaction
		Misc::Element trigger{};
		Misc::Reaction reaction{};
		decltype(makeAmplifyingFormula(std::declval<float>(), std::declval<Misc::Reaction>())) formula = makeAmplifyingFormula(multiplier, reaction);
	};

	namespace List {
		struct Amplifying {
			static constexpr auto forwardVape = Reaction::Amplifying{
				.name = "Forward Vape",
				.multiplier = 2.f,
				.trigger = Misc::Element::hydro,
				.reaction = Misc::Reaction::vape,
			};
			static constexpr auto reverseVape = Reaction::Amplifying{
				.name = "Reverse Vape",
				.multiplier = 1.5f,
				.trigger = Misc::Element::pyro,
				.reaction = Misc::Reaction::vape,
			};
			static constexpr auto forwardMelt = Reaction::Amplifying{
				.name = "Forward Melt",
				.multiplier = 2.f,
				.trigger = Misc::Element::pyro,
				.reaction = Misc::Reaction::melt,
			};
			static constexpr auto reverseMelt = Reaction::Amplifying{
				.name = "Reverse Melt",
				.multiplier = 1.5f,
				.trigger = Misc::Element::cryo,
				.reaction = Misc::Reaction::melt,
			};
		};
	}// namespace List
}// namespace Reaction