#pragma once

#include "array"
#include "formula/multiplier.hpp"
#include "formula/operators.hpp"
#include "formula/stat.hpp"
#include "misc/element.hpp"
#include "string_view"
#include "vector"


namespace Reaction {
	[[nodiscard]] static consteval auto makeTransformativeFormula(float multiplier) {
		constexpr auto levelMultiplier = Formula::LevelMultiplier{};
		constexpr auto emBonus = (16.f * Formula::Stat(Stat::em)) / (Formula::Stat(Stat::em) + 2000.f);
		// FIXME: add reaction bonus
		constexpr auto reactionBonus = 0.f;
		// FIXME: add res multiplier
		constexpr auto resMultiplier = 0.9f;
		// FIXME: add crit
		constexpr auto critMultiplier = 1.f;
		return Formula::Constant(multiplier) * levelMultiplier * (1.f + emBonus + reactionBonus) * resMultiplier * critMultiplier;
	}
	struct Transformative {
		std::string_view name;
		float multiplier;
		std::vector<Misc::Element> triggers;


		// const decltype(makeTransformativeFormula(std::declval<float>())) formula = makeTransformativeFormula(multiplier);
	};

	namespace List {
		struct Transformative {
			static inline Reaction::Transformative burning{
				.name = "Burning",
				.multiplier = 0.25f,
				.triggers{
					Misc::Element::pyro,
				},
			};
			static inline Reaction::Transformative superconduct{
				.name = "Superconduct",
				.multiplier = 0.5f,
				.triggers{
					Misc::Element::cryo,
					Misc::Element::electro,
				},
			};
			static inline Reaction::Transformative swirl{
				.name = "Swirl",
				.multiplier = 0.6f,
				.triggers{
					Misc::Element::anemo,
				},
			};
			static inline Reaction::Transformative electroCharged{
				.name = "Electro-Charged",
				.multiplier = 1.2f,
				.triggers{
					Misc::Element::electro,
					Misc::Element::hydro,
				},
			};
			static inline Reaction::Transformative shattered{
				.name = "Shattered",
				.multiplier = 1.5f,
				.triggers{
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
			static inline Reaction::Transformative overloaded{
				.name = "Overloaded",
				.multiplier = 2.0f,
				.triggers{
					Misc::Element::pyro,
					Misc::Element::electro,
				},
			};
			static inline Reaction::Transformative bloom{
				.name = "Bloom",
				.multiplier = 2.0f,
				.triggers{
					Misc::Element::hydro,
					Misc::Element::dendro,
				},
			};
			static inline Reaction::Transformative burgeon{
				.name = "Burgeon",
				.multiplier = 3.0f,
				.triggers{
					Misc::Element::pyro,
				},
			};
			static inline Reaction::Transformative hyperbloom{
				.name = "Hyperbloom",
				.multiplier = 3.0f,
				.triggers{
					Misc::Element::electro,
				},
			};

			[[nodiscard]] static consteval auto getMembers() {
				return std::array{
					&Reaction::List::Transformative::burning,
					&Reaction::List::Transformative::superconduct,
					&Reaction::List::Transformative::swirl,
					&Reaction::List::Transformative::electroCharged,
					&Reaction::List::Transformative::shattered,
					&Reaction::List::Transformative::overloaded,
					&Reaction::List::Transformative::bloom,
					&Reaction::List::Transformative::burgeon,
					&Reaction::List::Transformative::hyperbloom,
				};
			}
		};
	}// namespace List
}// namespace Reaction