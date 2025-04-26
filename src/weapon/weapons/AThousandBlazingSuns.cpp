#include "AThousandBlazingSuns.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::aThousandBlazingSuns{
	.key{12514},
	.goodKey{"AThousandBlazingSuns"},
	.name = "A Thousand Blazing Suns",
	.baseStats{
		.type = Misc::WeaponType::claymore,
		.rarity = 5,
		.baseAtk = 49.138,
		.atkCurve = Curves::WeaponGrow::ATTACK_303,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cr,
				.value = 0.024,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.2000, 0.2500, 0.3000, 0.3500, 0.4000});
		auto multiplier2 = WeaponMultiplier(false, {0.2800, 0.3500, 0.4200, 0.4900, 0.5600});

		auto passive1 = IsActive("aThousandBlazingSuns1");
		auto passive1CD = Requires(passive1, multiplier1);
		auto passive1Atk = Requires(passive1, multiplier2);

		auto passive2 = IsActive("aThousandBlazingSuns2");
		auto passive2CD = Requires(passive2, passive1CD * 0.75f);
		auto passive2Atk = Requires(passive2, passive1Atk * 0.75f);

		return Data::Setup{
			.mods{
				.preMod{
					.atk_ = passive1Atk + passive2Atk,
					.cd = passive1CD + passive2CD,
				},
			},
			.opts{
				Option::Boolean{
					.key = "aThousandBlazingSuns1",
					.name = "After using an Elemental Skill or Burst",
					.mods{
						.preMod{
							.atk_ = passive1Atk,
							.cd = passive1CD,
						},
					},
				},
				Option::Boolean{
					.key = "aThousandBlazingSuns2",
					.name = "The equipping character is in the Nightsoul's Blessing state",
					.mods{
						.preMod{
							.atk_ = passive2Atk,
							.cd = passive2CD,
						},
					},
				},
			},
		};
	},
};
