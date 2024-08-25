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
	this->preMods.baseAtk.modifiers.at(1) = Formula::Prefix(
		"Weapon Base",
		Formula::Custom([](const Stats::Loadout &source, const Stats::Loadout &, const Team &) {
			return getWeaponAtk(source.weapon);
		})
	);

	this->preMods.fromStat(base.substat.stat).modifiers.at(1) = Formula::Prefix(
		"Weapon Base",
		Formula::Custom(
			[](const Stats::Loadout &source, const Stats::Loadout &, const Team &) {
				return getWeaponSubstat(source.weapon);
			},
			Stats::isPercentage(base.substat.stat)
		)
	);
}
