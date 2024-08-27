#pragma once

#include "array"
#include "formula/multiplier.hpp"
#include "formula/operators.hpp"
#include "formula/stat.hpp"
#include "misc/element.hpp"
#include "string_view"


namespace Reaction {
	[[nodiscard]] static consteval auto makeAdditiveFormula(float multiplier) {
		constexpr auto levelMultiplier = Formula::LevelMultiplier{};
		constexpr auto emBonus = (5.f * Formula::Stat(Stat::em)) / (Formula::Stat(Stat::em) + 1200.f);
		// FIXME: add reaction bonus
		constexpr auto reactionBonus = 0.f;
		return Formula::Constant(multiplier) * levelMultiplier * (1.f + emBonus + reactionBonus);
	}
	struct Additive {
		std::string_view name;
		float multiplier;
		Misc::Element trigger;

		const decltype(makeAdditiveFormula(std::declval<float>())) formula = makeAdditiveFormula(multiplier);

		consteval Additive(std::string_view name, float multiplier, Misc::Element trigger) : name(name), multiplier(multiplier), trigger(trigger) {}
	};

	namespace List {
		struct Additive {
			static inline constexpr Reaction::Additive aggravate{
				"Aggravate",
				1.15f,
				Misc::Element::electro,
			};
			static inline constexpr Reaction::Additive spread{
				"Spread",
				1.25f,
				Misc::Element::dendro,
			};

			[[nodiscard]] static consteval auto getMembers() {
				return std::array{
					&Reaction::List::Additive::aggravate,
					&Reaction::List::Additive::spread,
				};
			}
		};
	}// namespace List
}// namespace Reaction