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
	this->stats.baseAtk.modifiers.at(0) = Formula::Prefix(
		"Weapon Base",
		Formula::Custom([](const Formula::Context &context) {
			return getWeaponAtk(context.source.weapon);
		})
	);

	this->stats.fromStat(base.substat.stat).modifiers.at(0) = Formula::Prefix(
		"Weapon Base",
		Formula::Custom(
			[](const Formula::Context &context) {
				return getWeaponSubstat(context.source.weapon);
			},
			Utils::isPercentage(base.substat.stat)
		)
	);
}
