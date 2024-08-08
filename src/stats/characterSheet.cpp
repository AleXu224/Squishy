#include "characterSheet.hpp"

#include "stats/sheet.hpp"

void Stats::CharacterSheet::init(Stats::Sheet &stats) {
	// HP
	addModifier(baseHp, [](const Stats::Sheet &stats) {
		return stats.character.base.getHpAt(stats.character.sheet.level, stats.character.sheet.ascension);
	});
	addModifier(stats.character.sheet.hp, [](const Stats::Sheet &stats) {
		return (stats.character.sheet.hp_.getTotal(stats) + 1.f) * stats.character.sheet.baseHp.getTotal(stats);
	});

	// ATK
	addModifier(baseAtk, [](const Stats::Sheet &stats) {
		return stats.character.base.getAtkAt(stats.character.sheet.level, stats.character.sheet.ascension);
	});
	addModifier(stats.character.sheet.atk, [](const Stats::Sheet &stats) {
		return (stats.character.sheet.atk_.getTotal(stats) + 1.f) * stats.character.sheet.baseAtk.getTotal(stats);
	});

	// DEF
	addModifier(baseDef, [](const Stats::Sheet &stats) {
		return stats.character.base.getDefAt(stats.character.sheet.level, stats.character.sheet.ascension);
	});
	addModifier(stats.character.sheet.def, [](const Stats::Sheet &stats) {
		return (stats.character.sheet.def_.getTotal(stats) + 1.f) * stats.character.sheet.baseDef.getTotal(stats);
	});

	// Ascension stat
	addModifier(stats.character.sheet.fromStat(stats.character.base.ascensionStat), [](const Stats::Sheet &stats) {
		return stats.character.base.getAscensionStatAt(stats.character.sheet.ascension);
	});

	linkWeaponAndArtifacts(stats);
}

void Stats::CharacterSheet::linkWeaponAndArtifacts(Stats::Sheet &stats) {
	for (auto [cPtr, wPtr, aPtr]: std::views::zip(
			 Stats::getSheetValuesMembers<CharacterSheet>(),
			 Stats::getSheetValuesMembers<WeaponSheet>(),
			 Stats::getSheetValuesMembers<ArtifactSheet>()
		 )) {
		auto &cStat = std::invoke(cPtr, stats.character.sheet);

		addModifier(cStat, [wPtr, aPtr](const Stats::Sheet &stats) {
			return std::invoke(wPtr, stats.weapon.sheet).getTotal(stats) + std::invoke(aPtr, stats.artifact.sheet).getTotal(stats);
		});
	}

	for (auto [cPtr, wPtr, aPtr]: std::views::zip(
			 Stats::getSheetElementsMembers<CharacterSheet>(),
			 Stats::getSheetElementsMembers<WeaponSheet>(),
			 Stats::getSheetElementsMembers<ArtifactSheet>()
		 )) {
		for (auto ssvPtr: SSV::getMembers()) {
			auto cStat = std::invoke(ssvPtr, std::invoke(cPtr, stats.character.sheet));

			addModifier(cStat, [wPtr, aPtr, ssvPtr](const Stats::Sheet &stats) {
				return std::invoke(ssvPtr, std::invoke(wPtr, stats.weapon.sheet)).getTotal(stats) +
					   std::invoke(ssvPtr, std::invoke(aPtr, stats.artifact.sheet)).getTotal(stats);
			});
		}
	}

	for (auto [cPtr, wPtr, aPtr]: std::views::zip(
			 Stats::getSheetAttackSourceMembers<CharacterSheet>(),
			 Stats::getSheetAttackSourceMembers<WeaponSheet>(),
			 Stats::getSheetAttackSourceMembers<ArtifactSheet>()
		 )) {
		for (auto ssvPtr: SSV::getMembers()) {
			auto cStat = std::invoke(ssvPtr, std::invoke(cPtr, stats.character.sheet));

			addModifier(cStat, [wPtr, aPtr, ssvPtr](const Stats::Sheet &stats) {
				return std::invoke(ssvPtr, std::invoke(wPtr, stats.weapon.sheet)).getTotal(stats) +
					   std::invoke(ssvPtr, std::invoke(aPtr, stats.artifact.sheet)).getTotal(stats);
			});
		}
	}
}
