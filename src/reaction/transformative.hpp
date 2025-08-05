#pragma once

#include "formula/clamp.hpp"
#include "formula/enemy.hpp"
#include "formula/multiplier.hpp"
#include "formula/operators.hpp"
#include "misc/element.hpp"
#include "string_view"


namespace Reaction {
	using namespace Formula::Operators;
	[[nodiscard]] static Formula::FloatNode makeTransformativeFormula(const Stats::Sheet<Formula::FloatNode>::_SkillValue &modifier, float multiplier, Misc::Element element) {
		constexpr auto levelMultiplier = Formula::LevelMultiplier{};
		auto emBonus = (Formula::ConstantFlat(16.f) * Modifiers::total().em) / (Modifiers::total().em + Formula::ConstantFlat(2000.f));
		auto reactionBonus = modifier.DMG;
		auto resMultiplier = Formula::EnemyResMultiplier({}, element);
		auto critMultiplier = 1.f + Formula::Clamp(modifier.critRate, 0.f, 1.f) * modifier.critDMG;
		return Formula::Constant(multiplier) * levelMultiplier * (1.f + emBonus + reactionBonus) * resMultiplier * critMultiplier;
	}

	[[nodiscard]] static Formula::FloatNode makeLunarTransformativeFormula(const Stats::Sheet<Formula::FloatNode>::_SkillValue &modifier, float multiplier, Misc::Element element) {
		constexpr auto levelMultiplier = Formula::LevelMultiplier{};
		auto emBonus = (Formula::ConstantFlat(6.f) * Modifiers::total().em) / (Modifiers::total().em + Formula::ConstantFlat(2000.f));
		auto reactionBaseMultiplier = modifier.multiplicativeDMG;
		auto reactionElevation = modifier.elevation;
		auto reactionBonus = modifier.DMG;
		auto resMultiplier = Formula::EnemyResMultiplier({}, element);
		auto critMultiplier = 1.f + Formula::Clamp(modifier.critRate + Modifiers::total().cr, 0.f, 1.f) * (modifier.critDMG + Modifiers::total().cd);
		return Formula::Constant(multiplier)
			 * levelMultiplier
			 * (1.f + reactionBaseMultiplier)
			 * (1.f + reactionElevation)
			 * (1.f + emBonus + reactionBonus)
			 * resMultiplier * critMultiplier;
	}

	struct Transformative {
		std::string_view name;
		float multiplier{};
		Misc::Element damageElement{};
		const Stats::Sheet<Formula::FloatNode>::_SkillValue &modifier;
		std::vector<Misc::Element> triggers;
		Formula::FloatNode formula = makeTransformativeFormula(modifier, multiplier, damageElement);
	};

	namespace List {
		struct Transformative {
			const static inline Reaction::Transformative burning = Reaction::Transformative{
				.name = "Burning",
				.multiplier = 0.25f,
				.damageElement = Misc::Element::pyro,
				.modifier = Modifiers::total().burning,
				.triggers{
					Misc::Element::pyro,
				},
			};
			const static inline Reaction::Transformative superconduct = Reaction::Transformative{
				.name = "Superconduct",
				.multiplier = 1.5f,
				.damageElement = Misc::Element::cryo,
				.modifier = Modifiers::total().superconduct,
				.triggers{
					Misc::Element::cryo,
					Misc::Element::electro,
				},
			};
			const static inline Reaction::Transformative pyroSwirl = Reaction::Transformative{
				.name = "Pyro Swirl",
				.multiplier = 0.6f,
				.damageElement = Misc::Element::pyro,
				.modifier = Modifiers::total().swirl,
				.triggers{
					Misc::Element::anemo,
				},
			};
			const static inline Reaction::Transformative hydroSwirl = Reaction::Transformative{
				.name = "Hydro Swirl",
				.multiplier = 0.6f,
				.damageElement = Misc::Element::hydro,
				.modifier = Modifiers::total().swirl,
				.triggers{
					Misc::Element::anemo,
				},
			};
			const static inline Reaction::Transformative electroSwirl = Reaction::Transformative{
				.name = "Electro Swirl",
				.multiplier = 0.6f,
				.damageElement = Misc::Element::electro,
				.modifier = Modifiers::total().swirl,
				.triggers{
					Misc::Element::anemo,
				},
			};
			const static inline Reaction::Transformative cryoSwirl = Reaction::Transformative{
				.name = "Cryo Swirl",
				.multiplier = 0.6f,
				.damageElement = Misc::Element::cryo,
				.modifier = Modifiers::total().swirl,
				.triggers{
					Misc::Element::anemo,
				},
			};
			const static inline Reaction::Transformative electroCharged = Reaction::Transformative{
				.name = "Electro-Charged",
				.multiplier = 2.f,
				.damageElement = Misc::Element::electro,
				.modifier = Modifiers::total().electroCharged,
				.triggers{
					Misc::Element::electro,
					Misc::Element::hydro,
				},
			};
			const static inline Reaction::Transformative shattered = Reaction::Transformative{
				.name = "Shattered",
				.multiplier = 3.f,
				.damageElement = Misc::Element::physical,
				.modifier = Modifiers::total().shattered,
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
			const static inline Reaction::Transformative overloaded = Reaction::Transformative{
				.name = "Overloaded",
				.multiplier = 2.75f,
				.damageElement = Misc::Element::pyro,
				.modifier = Modifiers::total().overloaded,
				.triggers{
					Misc::Element::pyro,
					Misc::Element::electro,
				},
			};
			const static inline Reaction::Transformative bloom = Reaction::Transformative{
				.name = "Bloom",
				.multiplier = 2.0f,
				.damageElement = Misc::Element::dendro,
				.modifier = Modifiers::total().bloom,
				.triggers{
					Misc::Element::hydro,
					Misc::Element::dendro,
				},
			};
			const static inline Reaction::Transformative burgeon = Reaction::Transformative{
				.name = "Burgeon",
				.multiplier = 3.0f,
				.damageElement = Misc::Element::dendro,
				.modifier = Modifiers::total().burgeon,
				.triggers{
					Misc::Element::pyro,
				},
			};
			const static inline Reaction::Transformative hyperbloom = Reaction::Transformative{
				.name = "Hyperbloom",
				.multiplier = 3.0f,
				.damageElement = Misc::Element::dendro,
				.modifier = Modifiers::total().hyperbloom,
				.triggers{
					Misc::Element::electro,
				},
			};

			// Lunar
			const static inline Reaction::Transformative lunarCharged = Reaction::Transformative{
				.name = "Lunar-Charged",
				.multiplier = 1.8f,
				.damageElement = Misc::Element::electro,
				.modifier = Modifiers::total().lunarCharged,
				.triggers{
					Misc::Element::electro,
					Misc::Element::hydro,
				},
				.formula = makeLunarTransformativeFormula(Modifiers::total().lunarCharged, 1.8f, Misc::Element::electro),
			};
		};
	}// namespace List
}// namespace Reaction