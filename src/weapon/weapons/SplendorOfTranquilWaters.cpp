#include "SplendorOfTranquilWaters.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::splendorOfTranquilWaters{
	.key{11513},
	.goodKey = "SplendorOfTranquilWaters",
	.name = "Splendor of Tranquil Waters",
	.baseStats{
		.type = Misc::WeaponType::sword,
		.rarity = 5,
		.baseAtk = 44.336,
		.atkCurve = Curves::WeaponGrow::ATTACK_304,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cd,
				.value = 0.192,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.0800, 0.1000, 0.1200, 0.1400, 0.1600});
		auto multiplier4 = WeaponMultiplier(false, {0.1400, 0.1750, 0.2100, 0.2450, 0.2800});

		auto hpChangedCount = GetInt("splendorOfTranquiWatersHpChanged");
		auto hpOthersChangedCount = GetInt("splendorOfTranquiWatersOthersHpChanged");
		auto skillDmgIncrease = hpChangedCount * multiplier1;
		auto hpIncrease = hpOthersChangedCount * multiplier4;

		return Data::Setup{
			.mods{
				.preMod{
					.hp_ = hpIncrease,
					.skill{
						.DMG = skillDmgIncrease,
					},
				},
			},
			.opts{
				Option::ValueList{
					.key = "splendorOfTranquiWatersHpChanged",
					.prefix = "Current character's HP increases or decreases",
					.values{1, 2, 3},
					.mods{
						.preMod{
							.skill{
								.DMG = skillDmgIncrease,
							},
						},
					},
				},
				Option::ValueList{
					.key = "splendorOfTranquiWatersOthersHpChanged",
					.prefix = "Party members' HP increases or decreases",
					.teamBuff = true,
					.values{1, 2},
					.mods{
						.preMod{
							.hp_ = hpIncrease,
						},
					},
				},
			},
		};
	},
};
