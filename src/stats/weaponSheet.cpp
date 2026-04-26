#include "weaponSheet.hpp"

#include "formula/custom.hpp"
#include "formula/prefix.hpp"
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
	this->stats.baseAtk.modifiers.at(0) = Formula::Prefix{
		.prefix = "Weapon Base",
		.val = Formula::Custom{
			.foldFunc = [](const Formula::Context &context, const Formula::FoldArgs &args) -> Formula::FloatNode {
				return Formula::ConstantFlat{.value = getWeaponAtk(*context.source.loadout().weapon)};
			},
			.func = [](const Formula::Context &context) {
				return getWeaponAtk(*context.source.loadout().weapon);
			}
		}
	};

	if (base.subStat.has_value()) {
		this->stats.fromStat(base.subStat.value().stat.stat.value()).modifiers.at(0) = Formula::Prefix{
			.prefix = "Weapon Base",
			.val = Formula::Custom{
				.foldFunc = [](const Formula::Context &context, const Formula::FoldArgs &args) -> Formula::FloatNode {
					return Formula::ConstantFlat{.value = getWeaponSubstat(*context.source.loadout().weapon)};
				},
				.func = [](const Formula::Context &context) {
					return getWeaponSubstat(*context.source.loadout().weapon);
				},
				.isPercentage = Utils::isPercentage(base.subStat.value().stat.stat)
			}
		};
	}
}
