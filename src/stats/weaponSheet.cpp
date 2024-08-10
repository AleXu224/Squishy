#include "weaponSheet.hpp"

#include "formula/formula.hpp"
#include "stats/sheet.hpp"
#include "stats/weapon.hpp"


constexpr auto getWeaponAtk = [](const Stats::Weapon &stats) {
	return stats.base.getAtkAt(stats.sheet.level, stats.sheet.ascension);
};
constexpr auto getWeaponSubstat = [](const Stats::Weapon &stats) {
	return stats.base.getSubstatAt(stats.sheet.level);
};

Stats::WeaponSheet::WeaponSheet(const WeaponBase &base) {
	addModifier(
		baseAtk,
		Formula::Custom("Weapon Base", [](const Stats::Sheet &stats) {
			return getWeaponAtk(stats.weapon);
		})
	);

	addModifier(
		fromStat(base.substat.stat),
		Formula::Custom(
			"Weapon Base", [](const Stats::Sheet &stats) {
				return getWeaponSubstat(stats.weapon);
			},
			Stats::isPercentage(base.substat.stat)
		)
	);
}
