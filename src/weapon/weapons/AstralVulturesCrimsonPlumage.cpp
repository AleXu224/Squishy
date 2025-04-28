#include "AstralVulturesCrimsonPlumage.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::astralVulturesCrimsonPlumage{
	.key{15514},
	.goodKey{"AstralVulturesCrimsonPlumage"},
	.name = "Astral Vulture's Crimson Plumage",
	.baseStats{
		.type = Misc::WeaponType::bow,
		.rarity = 5,
		.baseAtk = 45.936,
		.atkCurve = Curves::WeaponGrow::ATTACK_301,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cd,
				.value = 0.144,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.2400, 0.3000, 0.3600, 0.4200, 0.4800});
		auto multiplier2 = WeaponMultiplier(true, {0.0000, 0.0000, 0.0000, 0.0000, 0.0000});
		auto multiplier3 = WeaponMultiplier(true, {0.2000, 0.2500, 0.3000, 0.3500, 0.4000});
		auto multiplier4 = WeaponMultiplier(true, {0.4800, 0.6000, 0.7200, 0.8400, 0.9600});
		auto multiplier5 = WeaponMultiplier(true, {0.1000, 0.1250, 0.1500, 0.1750, 0.2000});
		auto multiplier6 = WeaponMultiplier(true, {0.2400, 0.3000, 0.3600, 0.4200, 0.4800});

		auto cond = IsActive("astralVulturesCrimsonPlumage");
		auto condBuff = Requires(cond, multiplier1);

		auto otherElementCount = Min(OtherElementCount{}, 2);

		auto chargedBuff = Evaluator(Index(otherElementCount, true, std::array{multiplier2, multiplier3, multiplier4}));
		auto burstBuff = Evaluator(Index(otherElementCount, true, std::array{multiplier2, multiplier5, multiplier6}));

		return Data::Setup{
			.mods{
				.preMod{
					.atk_ = condBuff,
					.charged{.DMG = chargedBuff},
					.burst{.DMG = burstBuff},
				},
			},
			.opts{
				Option::Boolean{
					.key = "astralVulturesCrimsonPlumage",
					.name = "After triggering a Swirl reaction",
				},
			},
		};
	},
};
