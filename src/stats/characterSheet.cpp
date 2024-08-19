#include "characterSheet.hpp"

#include "characterSheetUtils.hpp"
#include "formula/formula.hpp"
#include "stats/loadout.hpp"
#include "utils.hpp"


void Stats::CharacterSheet::init(Stats::Loadout &stats) {
	// HP
	this->stats.preMods.baseHp.modifiers.at(2) = Formula::Custom("Character Base", [](const Stats::Loadout &stats) {
		return stats.character.base.getHpAt(stats.character.sheet.level, stats.character.sheet.ascension);
	});
	this->stats.preMods.hp.modifiers.at(2) = (Formula::Stat(Stat::hp_) + 1.f) * Formula::Stat(Stat::baseHp);

	// ATK
	this->stats.preMods.baseAtk.modifiers.at(2) = Formula::Custom("Character Base", [](const Stats::Loadout &stats) {
		return stats.character.base.getAtkAt(stats.character.sheet.level, stats.character.sheet.ascension);
	});
	this->stats.preMods.atk.modifiers.at(2) = (Formula::Stat(Stat::atk_) + 1.f) * Formula::Stat(Stat::baseAtk);

	// DEF
	this->stats.preMods.baseDef.modifiers.at(2) = Formula::Custom("Character Base", [](const Stats::Loadout &stats) {
		return stats.character.base.getDefAt(stats.character.sheet.level, stats.character.sheet.ascension);
	});
	this->stats.preMods.def.modifiers.at(2) = (Formula::Stat(Stat::def_) + 1.f) * Formula::Stat(Stat::baseDef);

	// Ascension stat
	this->stats.preMods.fromStat(stats.character.base.ascensionStat).modifiers.at(2) = Formula::Custom(
		"Character Base", [](const Stats::Loadout &stats) {
			return stats.character.base.getAscensionStatAt(stats.character.sheet.ascension);
		},
		Stats::isPercentage(stats.character.base.ascensionStat)
	);
	linkWeaponAndArtifacts(stats);
}

void Stats::CharacterSheet::linkWeaponAndArtifacts(Stats::Loadout &stats) {
	squi::utils::iterateTuple(CharacterSheetUtils::sheetValueModifiers, [&](const auto &lbd) {
		const auto &[pre, post] = lbd;

		const auto &[preStat, preMod] = pre;
		std::invoke(preStat, stats.character.sheet.stats.preMods).modifiers.at(1) = preMod;
		const auto &[postStat, postMod] = post;
		std::invoke(postStat, stats.character.sheet.stats.postMods).modifiers.at(1) = postMod;
	});

	squi::utils::iterateTuple(CharacterSheetUtils::sheetElementModifiers, [&](const auto &lbd) {
		const auto &[pre, post] = lbd;

		const auto &[preCharacter, preMod] = pre;
		const auto &[preSkill, preStat] = preCharacter;
		std::invoke(preStat, std::invoke(preSkill, stats.character.sheet.stats.preMods)).modifiers.at(1) = preMod;
		const auto &[postCharacter, postMod] = post;
		const auto &[postSkill, postStat] = postCharacter;
		std::invoke(postStat, std::invoke(postSkill, stats.character.sheet.stats.postMods)).modifiers.at(1) = postMod;
	});

	squi::utils::iterateTuple(CharacterSheetUtils::sheetAttacksModifiers, [&](const auto &lbd) {
		const auto &[pre, post] = lbd;

		const auto &[preCharacter, preMod] = pre;
		const auto &[preSkill, preStat] = preCharacter;
		std::invoke(preStat, std::invoke(preSkill, stats.character.sheet.stats.preMods)).modifiers.at(1) = preMod;
		const auto &[postCharacter, postMod] = post;
		const auto &[postSkill, postStat] = postCharacter;
		std::invoke(postStat, std::invoke(postSkill, stats.character.sheet.stats.postMods)).modifiers.at(1) = postMod;
	});

	for (auto [preMember, postMember]: std::views::zip(Stats::getSheetValuesMembers<CharacterSheetUtils::_Pre<CharacterSheet>>(), Stats::getSheetValuesMembers<CharacterSheetUtils::_Post<CharacterSheet>>())) {
		std::invoke(postMember, this->stats.postMods).modifiers.at(2) = Formula::StatPtr<CharacterSheetUtils::_Pre<CharacterSheet>, CharacterSheet>(preMember, CharacterSheetUtils::_PreS<CharacterSheet>());
	}
	for (auto [preMember, postMember]: std::views::zip(
			 std::views::cartesian_product(
				 Stats::getSheetElementsMembers<CharacterSheetUtils::_Pre<CharacterSheet>>(),
				 CharacterSheetUtils::_Pre<CharacterSheet>::_SkillValue::getMembers()
			 ),
			 std::views::cartesian_product(
				 Stats::getSheetElementsMembers<CharacterSheetUtils::_Post<CharacterSheet>>(),
				 CharacterSheetUtils::_Post<CharacterSheet>::_SkillValue::getMembers()
			 )
		 )) {
			auto &[preSkill, preStat] = preMember;
			auto &[postSkill, postStat] = postMember;
		std::invoke(postStat, std::invoke(postSkill, this->stats.postMods)).modifiers.at(2) = Formula::SkillPtr<CharacterSheetUtils::_Pre<CharacterSheet>, CharacterSheet>(preSkill, preStat, CharacterSheetUtils::_PreS<CharacterSheet>());
	}
	for (auto [preMember, postMember]: std::views::zip(
			 std::views::cartesian_product(
				 Stats::getSheetAttackSourceMembers<CharacterSheetUtils::_Pre<CharacterSheet>>(),
				 CharacterSheetUtils::_Pre<CharacterSheet>::_SkillValue::getMembers()
			 ),
			 std::views::cartesian_product(
				 Stats::getSheetAttackSourceMembers<CharacterSheetUtils::_Post<CharacterSheet>>(),
				 CharacterSheetUtils::_Post<CharacterSheet>::_SkillValue::getMembers()
			 )
		 )) {
			auto &[preSkill, preStat] = preMember;
			auto &[postSkill, postStat] = postMember;
		std::invoke(postStat, std::invoke(postSkill, this->stats.postMods)).modifiers.at(2) = Formula::SkillPtr<CharacterSheetUtils::_Pre<CharacterSheet>, CharacterSheet>(preSkill, preStat, CharacterSheetUtils::_PreS<CharacterSheet>());
	}
}
