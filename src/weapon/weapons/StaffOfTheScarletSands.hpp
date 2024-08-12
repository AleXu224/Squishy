#pragma once

#include "weapon/data.hpp"

namespace Weapon::Datas {
	const inline Weapon::Data staffOfTheScarletSands{
		.key = 13511,
		.name = "Staff of the Scarlet Sands",
		.baseStats{
			.type = WeaponType::polearm,
			.baseAtk = 44.3358,
			.substat{
				.stat = Stat::cr,
				.value = 0.096,
			},
			.atkCurve = WeaponCurveType::GROWTH_CURVE_ATTACK_304,
			.substatCurve = WeaponCurveType::GROWTH_CURVE_CRITICAL_301,
			.ascensionUpgrade{
				0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7
			}
		},
		.multipliers{
			{0.52, 0.65, 0.78, 0.91, 1.04},
			{0.28, 0.35, 0.42, 0.49, 0.56},
		},
		.condsSetup = [](Weapon::Data::CondsSetup data) -> Conditional::WeaponList {
			return {
				Conditional::ValueList{
					.key = "eHits",
					.prefix = "Elemental Skill hits",
					.values{1, 2, 3},
				}
			};
		},
		.modsSetup = [](Weapon::Data::ModsSetup data) {
			Stats::addModifier(data.stats.sheet.atk, Formula::WeaponMultiplier(0) * Formula::Stat(Stat::em));

			Stats::addModifier(data.stats.sheet.atk, Formula::WeaponMultiplier(1) * Formula::Stat(Stat::em) * Formula::ConditionalValue(Conditional::Location::weapon, "eHits"));
		},
		.nodeSetup = [](Weapon::Data::NodeSetup data) -> Node::WeaponList {
			return Node::WeaponList{};
		},
	};
}// namespace Weapon::Datas
