#include "weaponSheet.hpp"

#include "stats/sheet.hpp"
#include "stats/weapon.hpp"


Stats::WeaponSheet::WeaponSheet(const WeaponBase &base) {
	baseAtkInt.modifiers.emplace_back([&base](const Stats::Weapon &stats) {
		return base.getAtkAt(stats.sheet.level, stats.sheet.ascension);
	});
	subStat.modifiers.emplace_back([&base](const Stats::Weapon &stats) {
		return base.getSubstatAt(stats.sheet.level);
	});

	addModifier(baseAtk, [](const Stats::Sheet &stats) {
		return stats.weapon.sheet.baseAtkInt.getTotal(stats.weapon);
	});
	addModifier(fromStat(base.substat.stat), [](const Stats::Sheet &stats) {
		return stats.weapon.sheet.subStat.getTotal(stats.weapon);
	});
}
