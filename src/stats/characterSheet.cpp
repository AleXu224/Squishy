#include "characterSheet.hpp"

#include "stats/sheet.hpp"

void Stats::CharacterSheet::init(Stats::Sheet &stats) {
	stats.character.sheet.hp.modifiers.emplace_back([](const Stats::Sheet &stats) {
		return (stats.character.sheet.hp_.getTotal(stats) + 1.f) * stats.character.base.getHpAt(stats.character.sheet.level, stats.character.sheet.ascension);
	});
	stats.character.sheet.atk.modifiers.emplace_back([](const Stats::Sheet &stats) {
		return (stats.character.sheet.atk_.getTotal(stats) + 1.f) *
			   (stats.character.base.getAtkAt(stats.character.sheet.level, stats.character.sheet.ascension) +
				stats.weapon.base.getAtkAt(stats.weapon.sheet.level, stats.weapon.sheet.ascension));
	});
	stats.character.sheet.def.modifiers.emplace_back([](const Stats::Sheet &stats) {
		return (stats.character.sheet.def_.getTotal(stats) + 1.f) * stats.character.base.getDefAt(stats.character.sheet.level, stats.character.sheet.ascension);
	});
	stats.character.sheet.fromStat(stats.character.base.ascensionStat).modifiers.emplace_back([](const Stats::Sheet &stats) {
		return stats.character.base.getAscensionStatAt(stats.character.sheet.ascension);
	});
}
