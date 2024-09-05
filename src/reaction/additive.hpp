#pragma once

#include "array"
#include "formula/multiplier.hpp"
#include "formula/operators.hpp"
#include "formula/stat.hpp"
#include "misc/element.hpp"
#include "string_view"


namespace Reaction {
	using _postMods = decltype(Stats::CharacterSheet::postMods);
	[[nodiscard]] static consteval auto makeAdditiveFormula(float multiplier, _postMods::_SkillValue _postMods:: *skill) {
		constexpr auto levelMultiplier = Formula::LevelMultiplier{};
		constexpr auto emBonus = (5.f * Formula::CharacterStat(Stat::em)) / (Formula::CharacterStat(Stat::em) + 1200.f);
		auto reactionBonus = Formula::SkillPtr(&Stats::CharacterSheet::postMods, skill, &_postMods::_SkillValue::DMG);
		return Formula::Constant(multiplier) * levelMultiplier * (1.f + emBonus + reactionBonus);
	}
	struct Additive {
		std::string_view name;
		float multiplier;
		Misc::Element trigger;
		_postMods::_SkillValue _postMods:: *skill;

		const decltype(makeAdditiveFormula(std::declval<float>(), std::declval<_postMods::_SkillValue _postMods:: *>())) formula = makeAdditiveFormula(multiplier, skill);

		consteval Additive(std::string_view name, float multiplier, Misc::Element trigger, _postMods::_SkillValue _postMods:: *skill) : name(name), multiplier(multiplier), trigger(trigger), skill(skill) {}
	};

	namespace List {
		struct Additive {
			static inline constexpr Reaction::Additive aggravate{
				"Aggravate",
				1.15f,
				Misc::Element::electro,
				&_postMods::aggravate,
			};
			static inline constexpr Reaction::Additive spread{
				"Spread",
				1.25f,
				Misc::Element::dendro,
				&_postMods::spread,
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