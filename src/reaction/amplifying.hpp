#pragma once

#include "array"
#include "formula/operators.hpp"
#include "formula/stat.hpp"
#include "misc/element.hpp"
#include "string_view"


namespace Reaction {
	[[nodiscard]] static consteval auto makeAmplifyingFormula(float multiplier) {
		constexpr auto emBonus = 2.78f * (Formula::Stat(Stat::em) / (Formula::Stat(Stat::em) + 1400.f));
		// FIXME: add reaction bonus
		constexpr auto reactionBonus = 0.f;
		return Formula::Constant(multiplier) * (1 + emBonus + reactionBonus);
	}
	struct Amplifying {
		std::string_view name;
		float multiplier;
		// The final element applied that will trigger this reaction
		Misc::Element trigger;

		const decltype(makeAmplifyingFormula(std::declval<float>())) formula = makeAmplifyingFormula(multiplier);

		consteval Amplifying(std::string_view name, float multiplier, Misc::Element trigger) : name(name), multiplier(multiplier), trigger(trigger) {}
	};

	namespace List {
		struct Amplifying {
			static inline constexpr Reaction::Amplifying forwardVape{
				"Forward Vape",
				2.f,
				Misc::Element::hydro,
			};
			static inline constexpr Reaction::Amplifying reverseVape{
				"Reverse Vape",
				1.5f,
				Misc::Element::pyro,
			};
			static inline constexpr Reaction::Amplifying forwardMelt{
				"Forward Melt",
				2.f,
				Misc::Element::pyro,
			};
			static inline constexpr Reaction::Amplifying reverseMelt{
				"Reverse Melt",
				1.5f,
				Misc::Element::cryo,
			};

			[[nodiscard]] static consteval auto getMembers() {
				return std::array{
					&Reaction::List::Amplifying::forwardVape,
					&Reaction::List::Amplifying::reverseVape,
					&Reaction::List::Amplifying::forwardMelt,
					&Reaction::List::Amplifying::reverseMelt,
				};
			}
		};
	}// namespace List
}// namespace Reaction