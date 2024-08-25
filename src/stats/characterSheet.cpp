#include "characterSheet.hpp"

#include "characterSheetUtils.hpp"
#include "formula/formula.hpp"
#include "stats/loadout.hpp"
#include "stats/team.hpp"

void Stats::CharacterSheet::init(Stats::Loadout &stats) {
	// HP
	this->preMods.baseHp.modifiers.at(2) = Formula::Prefix(
		"Character Base",
		Formula::Custom([](const Stats::Loadout &source, const Stats::Loadout &, const Stats::Team &) {
			return source.character.base.getHpAt(source.character.sheet.level, source.character.sheet.ascension);
		})
	);
	this->preMods.hp.modifiers.at(2) = (Formula::Stat(Stat::hp_) + 1.f) * Formula::Stat(Stat::baseHp);

	// ATK
	this->preMods.baseAtk.modifiers.at(2) = Formula::Prefix(
		"Character Base",
		Formula::Custom([](const Stats::Loadout &source, const Stats::Loadout &, const Stats::Team &) {
			return source.character.base.getAtkAt(source.character.sheet.level, source.character.sheet.ascension);
		})
	);
	this->preMods.atk.modifiers.at(2) = (Formula::Stat(Stat::atk_) + 1.f) * Formula::Stat(Stat::baseAtk);

	// DEF
	this->preMods.baseDef.modifiers.at(2) = Formula::Prefix(
		"Character Base",
		Formula::Custom([](const Stats::Loadout &source, const Stats::Loadout &, const Stats::Team &) {
			return source.character.base.getDefAt(source.character.sheet.level, source.character.sheet.ascension);
		})
	);
	this->preMods.def.modifiers.at(2) = (Formula::Stat(Stat::def_) + 1.f) * Formula::Stat(Stat::baseDef);

	// Ascension stat
	this->preMods.fromStat(stats.character.base.ascensionStat).modifiers.at(2) = Formula::Prefix(
		"Character Base",
		Formula::Custom(
			[](const Stats::Loadout &source, const Stats::Loadout &, const Stats::Team &) {
				return source.character.base.getAscensionStatAt(source.character.sheet.ascension);
			},
			Stats::isPercentage(stats.character.base.ascensionStat)
		)
	);
	linkWeaponAndArtifacts();
}

void Stats::CharacterSheet::linkWeaponAndArtifacts() {
	constexpr auto addMods = []<class T>(CharacterSheet &stats, T val) {
		auto [values, skills] = val;
		const auto &[valuesCharacter, valuesMod] = values;
		const auto &[skillsCharacter, skillsMod] = skills;
		for (const auto &[valueCharacter, valueMod]: std::views::zip(valuesCharacter, valuesMod)) {
			std::invoke(valueCharacter.stat, std::invoke(valueCharacter.location, stats)).modifiers.at(1) = valueMod;
		}
		for (const auto &[skillCharacter, skillMod]: std::views::zip(skillsCharacter, skillsMod)) {
			std::invoke(skillCharacter.stat, std::invoke(skillCharacter.skill, std::invoke(skillCharacter.location, stats))).modifiers.at(1) = skillMod;
		}
	};
	addMods(*this, CharacterSheetUtils::getModifiers(&CharacterSheet::preMods, &WeaponSheet::preMods, &ArtifactSheet::preMods));
	addMods(*this, CharacterSheetUtils::getModifiers(&CharacterSheet::postMods, &WeaponSheet::postMods, &ArtifactSheet::postMods));
	addMods(*this, CharacterSheetUtils::getModifiers(&CharacterSheet::teamPreMods, &WeaponSheet::teamPreMods, &ArtifactSheet::teamPreMods));
	addMods(*this, CharacterSheetUtils::getModifiers(&CharacterSheet::teamPostMods, &WeaponSheet::teamPostMods, &ArtifactSheet::teamPostMods));

	for (const auto &[preMember, postMember]: std::views::zip(Stats::getSheetValuesMembers<decltype(CharacterSheet::preMods)>(), Stats::getSheetValuesMembers<decltype(CharacterSheet::postMods)>())) {
		std::invoke(postMember, this->postMods).modifiers.at(2) = Formula::StatPtr(&CharacterSheet::preMods, preMember);
	}
	for (const auto &[preMember, postMember]: std::views::zip(
			 std::views::cartesian_product(
				 Stats::getSheetSkillsMembers<decltype(CharacterSheet::preMods)>(),
				 decltype(CharacterSheet::preMods)::_SkillValue::getMembers()
			 ),
			 std::views::cartesian_product(
				 Stats::getSheetSkillsMembers<decltype(CharacterSheet::postMods)>(),
				 decltype(CharacterSheet::postMods)::_SkillValue::getMembers()
			 )
		 )) {
		const auto &[preSkill, preStat] = preMember;
		const auto &[postSkill, postStat] = postMember;
		std::invoke(postStat, std::invoke(postSkill, this->postMods)).modifiers.at(2) = Formula::SkillPtr(&CharacterSheet::preMods, preSkill, preStat);
	}
	constexpr auto addModsTeam = []<class T>(CharacterSheet &stats, T val) {
		auto [values, skills] = val;
		const auto &[valuesCharacter, valuesMod] = values;
		const auto &[skillsCharacter, skillsMod] = skills;
		for (const auto &[valueCharacter, valueMod]: std::views::zip(valuesCharacter, valuesMod)) {
			std::invoke(valueCharacter.stat, std::invoke(valueCharacter.location, stats)).modifiers.at(3) = valueMod;
		}
		for (const auto &[skillCharacter, skillMod]: std::views::zip(skillsCharacter, skillsMod)) {
			std::invoke(skillCharacter.stat, std::invoke(skillCharacter.skill, std::invoke(skillCharacter.location, stats))).modifiers.at(3) = skillMod;
		}
	};
	addModsTeam(*this, CharacterSheetUtils::getTeamModifiers(&CharacterSheet::preMods, &::Stats::Team::preMods));
	addModsTeam(*this, CharacterSheetUtils::getTeamModifiers(&CharacterSheet::postMods, &::Stats::Team::postMods));
}
