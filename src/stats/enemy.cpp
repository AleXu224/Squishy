#include "enemy.hpp"

#include "formula/operators.hpp"// IWYU pragma: keep
#include "formula/stat.hpp"     // IWYU pragma: keep
#include "formula/teamCharacter.hpp"

template<class T, class V>
[[nodiscard]] consteval auto getEnemyTeamFormulas(T teamArg, V charArg) {
	constexpr auto makeFormula = [](auto val) consteval {
		return Formula::TeamCharacter(0, val.makeFormula()) +
			   Formula::TeamCharacter(1, val.makeFormula()) +
			   Formula::TeamCharacter(2, val.makeFormula()) +
			   Formula::TeamCharacter(3, val.makeFormula());
	};

	return std::tuple{
		std::tuple{
			std::get<0>(Stats::getEnemySheetAllMembers(teamArg)),
			squi::utils::transformRange(
				std::get<0>(Stats::getEnemySheetAllMembers(charArg)),
				makeFormula
			),
		},
		std::tuple{
			std::get<1>(Stats::getEnemySheetAllMembers(teamArg)),
			squi::utils::transformRange(
				std::get<1>(Stats::getEnemySheetAllMembers(charArg)),
				makeFormula
			),
		},
	};
}

Stats::Enemy::Enemy() {
	constexpr auto addMods = []<class T>(Enemy &stats, T val) {
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

	addMods(*this, getEnemyTeamFormulas(&Enemy::sheet, &CharacterSheet::enemySheet));
}
