#include "weaponSheet.hpp"

#include "formula/formula.hpp"
#include "stats/loadout.hpp"
#include "stats/weapon.hpp"


constexpr auto getWeaponAtk = [](const Stats::Weapon &stats) {
	return stats.data->baseStats.getAtkAt(stats.sheet.level, stats.sheet.ascension);
};
constexpr auto getWeaponSubstat = [](const Stats::Weapon &stats) {
	return stats.data->baseStats.getSubstatAt(stats.sheet.level);
};

Stats::WeaponSheet::WeaponSheet(const WeaponBase &base) {
	this->stats.baseAtk.modifiers.at(0) = Formula::Prefix(
		"Weapon Base",
		Formula::Custom([](const Formula::Context &context) {
			return getWeaponAtk(*context.source.weapon);
		})
	);

	if (base.subStat.has_value()) {
		this->stats.fromStat(base.subStat.value().stat.stat.value()).modifiers.at(0) = Formula::Prefix(
			"Weapon Base",
			Formula::Custom(
				[](const Formula::Context &context) {
					return getWeaponSubstat(*context.source.weapon);
				},
				Utils::isPercentage(base.subStat.value().stat.stat)
			)
		);
	}
}
