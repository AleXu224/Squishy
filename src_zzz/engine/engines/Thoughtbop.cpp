#include "Thoughtbop.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::thoughtbop{
	.key{14149},
	.goodKey{"Thoughtbop"},
	.name = "Thoughtbop",
	.baseStats{
		.specialty = Misc::Specialty::support,
		.rarity = 5,
		.baseAtk = 48,
		.subStat{
			.stat = Stat::er_,
			.value = 0.240,
		},
	},
	.setup = []() {
		auto multiplier2 = EngineMultiplier(true, {0.125f, 0.14300000000000002f, 0.161f, 0.179f, 0.2f});
		auto multiplier3 = EngineMultiplier(true, {0.1f, 0.115f, 0.13f, 0.145f, 0.16f});

		auto stacks = GetFloat("thoughtbopStacks");
		auto dmgBuff = stacks * multiplier2;
		auto atkBuff = Requires{
			.requirement = stacks >= 2,
			.ret = multiplier3,
		};

		return Data::Setup{
			.mods{
				.teamCombat{
					.atk_ = atkBuff,
					.all{
						.DMG = dmgBuff,
					},
				},
			},
			.opts{
				Option::ValueSlider{
					.key = "thoughtbopStacks",
					.name = "After the equipper uses an EX Special Attack and deals Physical DMG",
					.teamBuff = true,
					.values{0, 1, 2},
					.mods{
						.teamCombat{
							.atk_ = atkBuff,
							.all{
								.DMG = dmgBuff,
							},
						},
					},
				},
			},
		};
	},
};
