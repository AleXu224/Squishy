#include "weaponSheet.hpp"

#include "formula/formula.hpp"
#include "stats/loadout.hpp"
#include "stats/weapon.hpp"


constexpr auto getWeaponAtk = [](const Stats::Weapon &stats) {
	return stats.base.getAtkAt(stats.sheet.level, stats.sheet.ascension);
};
constexpr auto getWeaponSubstat = [](const Stats::Weapon &stats) {
	return stats.base.getSubstatAt(stats.sheet.level);
};

Stats::WeaponSheet::WeaponSheet(const WeaponBase &base) {
	this->stats.preMods.baseAtk.modifiers.at(1) = Formula::Custom("Weapon Base", [](const Stats::Loadout &stats) {
		return getWeaponAtk(stats.weapon);
	});

	this->stats.preMods.fromStat(base.substat.stat).modifiers.at(1) = Formula::Custom(
		"Weapon Base",
		[](const Stats::Loadout &stats) {
			return getWeaponSubstat(stats.weapon);
		},
		Stats::isPercentage(base.substat.stat)
	);
}
