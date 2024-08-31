#pragma once

#include "array"
#include "formula/operators.hpp"
#include "formula/stat.hpp"
#include "misc/element.hpp"
#include "string_view"


namespace Reaction {
	using _postMods = decltype(Stats::CharacterSheet::postMods);
	[[nodiscard]] static consteval auto makeAmplifyingFormula(float multiplier, _postMods::_SkillValue _postMods:: *skill) {
		constexpr auto emBonus = 2.78f * (Formula::Stat(Stat::em) / (Formula::Stat(Stat::em) + 1400.f));
		auto reactionBonus = Formula::SkillPtr(&Stats::CharacterSheet::postMods, skill, &_postMods::_SkillValue::DMG);
		return Formula::Constant(multiplier) * (1 + emBonus + reactionBonus);
	}
	struct Amplifying {
		std::string_view name;
		float multiplier;
		// The final element applied that will trigger this reaction
		Misc::Element trigger;
		_postMods::_SkillValue _postMods:: *skill;

		const decltype(makeAmplifyingFormula(std::declval<float>(), std::declval<_postMods::_SkillValue _postMods:: *>())) formula = makeAmplifyingFormula(multiplier, skill);

		consteval Amplifying(
			std::string_view name,
			float multiplier,
			Misc::Element trigger,
			_postMods::_SkillValue _postMods:: *skill
		) : name(name),
			multiplier(multiplier),
			trigger(trigger),
			skill(skill) {}
	};

	namespace List {
		struct Amplifying {
			static inline constexpr Reaction::Amplifying forwardVape{
				"Forward Vape",
				2.f,
				Misc::Element::hydro,
				&_postMods::vape,
			};
			static inline constexpr Reaction::Amplifying reverseVape{
				"Reverse Vape",
				1.5f,
				Misc::Element::pyro,
				&_postMods::vape,
			};
			static inline constexpr Reaction::Amplifying forwardMelt{
				"Forward Melt",
				2.f,
				Misc::Element::pyro,
				&_postMods::melt,
			};
			static inline constexpr Reaction::Amplifying reverseMelt{
				"Reverse Melt",
				1.5f,
				Misc::Element::cryo,
				&_postMods::melt,
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