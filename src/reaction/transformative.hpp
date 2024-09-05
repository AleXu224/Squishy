#pragma once

#include "array"
#include "formula/clamp.hpp"
#include "formula/enemy.hpp"
#include "formula/multiplier.hpp"
#include "formula/operators.hpp"
#include "formula/stat.hpp"
#include "misc/element.hpp"
#include "string_view"


namespace Reaction {
	using _postMods = decltype(Stats::CharacterSheet::postMods);
	[[nodiscard]] static consteval auto makeTransformativeFormula(float multiplier, Misc::Element element, _postMods::_SkillValue _postMods:: *skill) {
		constexpr auto levelMultiplier = Formula::LevelMultiplier{};
		constexpr auto emBonus = (Formula::ConstantFlat(16.f) * Formula::CharacterStat(Stat::em)) / (Formula::CharacterStat(Stat::em) + Formula::ConstantFlat(2000.f));
		auto reactionBonus = Formula::SkillPtr(&Stats::CharacterSheet::postMods, skill, &_postMods::_SkillValue::DMG);
		auto resMultiplier = Formula::EnemyResMultiplier({}, element);
		auto critMultiplier = 1.f + Formula::Clamp(Formula::SkillPtr(&Stats::CharacterSheet::postMods, skill, &_postMods::_SkillValue::critRate), 0.f, 1.f) * Formula::SkillPtr(&Stats::CharacterSheet::postMods, skill, &_postMods::_SkillValue::critDMG);
		return Formula::Constant(multiplier) * levelMultiplier * (1.f + emBonus + reactionBonus) * resMultiplier * critMultiplier;
	}
	template<class Trigs>
	struct Transformative {
		std::string_view name;
		float multiplier;
		Misc::Element damageElement;
		_postMods::_SkillValue _postMods:: *stat;
		Trigs triggers;


		const decltype(makeTransformativeFormula(std::declval<float>(), std::declval<Misc::Element>(), std::declval<_postMods::_SkillValue _postMods:: *>())) formula = makeTransformativeFormula(multiplier, damageElement, stat);

		consteval Transformative(
			std::string_view name,
			float multiplier,
			Misc::Element damageElement,
			_postMods::_SkillValue _postMods:: *stat,
			Trigs triggers
		) : name(name),
			multiplier(multiplier),
			damageElement(damageElement),
			stat(stat),
			triggers(triggers) {}
	};

	namespace List {
		struct Transformative {
			static inline Reaction::Transformative burning{
				"Burning",
				0.25f,
				Misc::Element::pyro,
				&_postMods::burning,
				std::array{
					Misc::Element::pyro,
				},
			};
			static inline Reaction::Transformative superconduct{
				"Superconduct",
				0.5f,
				Misc::Element::cryo,
				&_postMods::superconduct,
				std::array{
					Misc::Element::cryo,
					Misc::Element::electro,
				},
			};
			static inline Reaction::Transformative pyroSwirl{
				"Pyro Swirl",
				0.6f,
				Misc::Element::pyro,
				&_postMods::swirl,
				std::array{
					Misc::Element::anemo,
				},
			};
			static inline Reaction::Transformative hydroSwirl{
				"Hydro Swirl",
				0.6f,
				Misc::Element::hydro,
				&_postMods::swirl,
				std::array{
					Misc::Element::anemo,
				},
			};
			static inline Reaction::Transformative electroSwirl{
				"Electro Swirl",
				0.6f,
				Misc::Element::electro,
				&_postMods::swirl,
				std::array{
					Misc::Element::anemo,
				},
			};
			static inline Reaction::Transformative cryoSwirl{
				"Cryo Swirl",
				0.6f,
				Misc::Element::cryo,
				&_postMods::swirl,
				std::array{
					Misc::Element::anemo,
				},
			};
			static inline Reaction::Transformative electroCharged{
				"Electro-Charged",
				1.2f,
				Misc::Element::electro,
				&_postMods::electroCharged,
				std::array{
					Misc::Element::electro,
					Misc::Element::hydro,
				},
			};
			static inline Reaction::Transformative shattered{
				"Shattered",
				1.5f,
				Misc::Element::physical,
				&_postMods::shattered,
				std::array{
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
				"Overloaded",
				2.0f,
				Misc::Element::pyro,
				&_postMods::overloaded,
				std::array{
					Misc::Element::pyro,
					Misc::Element::electro,
				},
			};
			static inline Reaction::Transformative bloom{
				"Bloom",
				2.0f,
				Misc::Element::dendro,
				&_postMods::bloom,
				std::array{
					Misc::Element::hydro,
					Misc::Element::dendro,
				},
			};
			static inline Reaction::Transformative burgeon{
				"Burgeon",
				3.0f,
				Misc::Element::dendro,
				&_postMods::burgeon,
				std::array{
					Misc::Element::pyro,
				},
			};
			static inline Reaction::Transformative hyperbloom{
				"Hyperbloom",
				3.0f,
				Misc::Element::dendro,
				&_postMods::hyperbloom,
				std::array{
					Misc::Element::electro,
				},
			};

			[[nodiscard]] static consteval auto getMembers() {
				return std::tuple{
					&Reaction::List::Transformative::burning,
					&Reaction::List::Transformative::superconduct,
					&Reaction::List::Transformative::pyroSwirl,
					&Reaction::List::Transformative::hydroSwirl,
					&Reaction::List::Transformative::electroSwirl,
					&Reaction::List::Transformative::cryoSwirl,
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