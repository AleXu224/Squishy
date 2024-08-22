#include "team.hpp"

#include "formula/teamCharacter.hpp"
#include "stats/characterSheet.hpp"


template<class T, class V>
[[nodiscard]] consteval auto getTeamFormulas(T teamArg, V charArg) {
	constexpr auto makeFormula = [](auto val) consteval {
		return Formula::TeamCharacter(0, val.makeFormula()) +
			   Formula::TeamCharacter(1, val.makeFormula()) +
			   Formula::TeamCharacter(2, val.makeFormula()) +
			   Formula::TeamCharacter(3, val.makeFormula());
	};

	return std::tuple{
		std::tuple{
			std::get<0>(Stats::getSheetAllMembers(teamArg)),
			squi::utils::transformRange(
				std::get<0>(Stats::getSheetAllMembers(charArg)),
				makeFormula
			),
		},
		std::tuple{
			std::get<1>(Stats::getSheetAllMembers(teamArg)),
			squi::utils::transformRange(
				std::get<1>(Stats::getSheetAllMembers(charArg)),
				makeFormula
			),
		},
	};
}

Stats::Team::Team() {
	constexpr auto addMods = []<class T>(Team &stats, T val) {
		auto [values, skills] = val;
		const auto &[valuesCharacter, valuesMod] = values;
		const auto &[skillsCharacter, skillsMod] = skills;
		for (const auto &[valueCharacter, valueMod]: std::views::zip(valuesCharacter, valuesMod)) {
			std::invoke(valueCharacter.stat, std::invoke(valueCharacter.location, stats)).modifiers.at(0) = valueMod;
		}
		for (const auto &[skillCharacter, skillMod]: std::views::zip(skillsCharacter, skillsMod)) {
			std::invoke(skillCharacter.stat, std::invoke(skillCharacter.skill, std::invoke(skillCharacter.location, stats))).modifiers.at(0) = skillMod;
		}
	};

	addMods(*this, getTeamFormulas(&Team::preMods, &CharacterSheet::teamPreMods));
	addMods(*this, getTeamFormulas(&Team::postMods, &CharacterSheet::teamPostMods));
}
