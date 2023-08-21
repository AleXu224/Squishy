#pragma once
#include "../IWeaponData.hpp"
#include "node.hpp"
#include <array>

namespace Squishy {
	static const IWeaponData Staff_of_the_Scarlet_Sands = {
		.name = "Staff of the Scarlet Sands",
		.baseStats{
			.baseATK = 44.3358,
			.subStatValue = 0.096,
			.subStat = ArtifactStat::CritRate,
			.type = WeaponType::Polearm,
			.atkCurve = WeaponGrowCurve::ATTACK_304,
			.subStatCurve = WeaponGrowCurve::CRITICAL_301,
			.atkUpgrade{0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7},
		},
		.conditionalsSetup = [](StatSheet &sheet) -> IWeaponData::Conditionals {
			return {};
		},
		.modsSetup = [](StatSheet &sheet, const IWeaponData::Conditionals &conditionals) -> void {
			sheet.stats.ATK.addTotal([](const StatSheet &sheet) {
				constexpr std::array<float, 5> heatHazeatHorizonsEnd{0.52f, 0.65f, 0.78f, 0.91f, 1.04f};
				return sheet.stats.EM.getTotal(sheet) * heatHazeatHorizonsEnd.at(sheet.weaponRefinement - 1);
			});
		},
		.nodeSetup = [](StatSheet &sheet, const IWeaponData::Conditionals &conditionals) -> std::vector<Node> {
			return {
				StatModifierNode{
					.name = "ATK",
					.modifier = [](const StatSheet &sheet) {
						constexpr std::array<float, 5> heatHazeatHorizonsEnd{0.52f, 0.65f, 0.78f, 0.91f, 1.04f};
						return sheet.stats.EM.getTotal(sheet) * heatHazeatHorizonsEnd.at(sheet.weaponRefinement - 1);
					},
				},
			};
		},
	};
}