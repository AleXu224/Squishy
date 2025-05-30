#include "weaponSheet.hpp"

#include "formula/formula.hpp"
#include "stats/loadout.hpp"
#include "stats/weapon.hpp"
#include "weapon/data.hpp"


constexpr auto getWeaponAtk = [](const Stats::Weapon &stats) {
	return stats.data->baseStats.getAtkAt(stats.sheet.level, stats.sheet.ascension);
};
constexpr auto getWeaponSubstat = [](const Stats::Weapon &stats) {
	return stats.data->baseStats.getSubstatAt(stats.sheet.level);
};

Stats::WeaponSheet::WeaponSheet(const WeaponBase &base) {
	this->stats.baseAtk.modifiers.at(0) = Formula::Prefix(
		"Weapon Base",
		Formula::Custom(
			[](const Formula::Context &context) -> Formula::Compiled::FloatNode {
				return Formula::Compiled::ConstantFloat{.value = getWeaponAtk(*context.source.loadout().weapon)};
			},
			[](const Formula::Context &context) {
				return getWeaponAtk(*context.source.loadout().weapon);
			}
		)
	);

	if (base.subStat.has_value()) {
		this->stats.fromStat(base.subStat.value().stat.stat.value()).modifiers.at(0) = Formula::Prefix(
			"Weapon Base",
			Formula::Custom(
				[](const Formula::Context &context) -> Formula::Compiled::FloatNode {
					return Formula::Compiled::ConstantFloat{.value = getWeaponSubstat(*context.source.loadout().weapon)};
				},
				[](const Formula::Context &context) {
					return getWeaponSubstat(*context.source.loadout().weapon);
				},
				Utils::isPercentage(base.subStat.value().stat.stat)
			)
		);
	}
}
