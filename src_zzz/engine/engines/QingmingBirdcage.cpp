#include "QingmingBirdcage.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::qingmingBirdcage{
	.key{14137},
	.goodKey{"QingmingBirdcage"},
	.name = "Qingming Birdcage",
	.baseStats{
		.specialty = Misc::Specialty::rupture,
		.rarity = 5,
		.baseAtk = 50,
		.subStat{
			.stat = Stat::hp_,
			.value = 0.120,
		},
	},
	.setup = []() {
		auto multiplier1 = EngineMultiplier(true, {0.2f, 0.23f, 0.26f, 0.29f, 0.32f});
		auto multiplier2 = EngineMultiplier(true, {0.08f, 0.092f, 0.104f, 0.116f, 0.128f});
		auto multiplier3 = EngineMultiplier(true, {0.1f, 0.115f, 0.13f, 0.145f, 0.16f});

		auto stacks = GetFloat("qingmingBirdcageStacks");
		auto etherDmgBuff = multiplier2 * stacks;
		auto skillDmgBuff = Requires{
			.requirement = AgentAttribute{} == ConstantBase<Misc::Attribute>{.value = Misc::Attribute::ether},
			.ret = multiplier3 * stacks,
		};

		return Data::Setup{
			.mods{
				.combat{
					.cr = multiplier1,
					.ether{
						.DMG = etherDmgBuff,
					},
					.exspecial{
						.DMG = skillDmgBuff,
					},
					.ultimate{
						.DMG = skillDmgBuff,
					},
				},
			},
			.opts{
				Option::ValueSlider{
					.key = "qingmingBirdcageStacks",
					.name = "Equipper launches an EX Special Attack",
					.values{0.f, 1.f, 2.f},
					.mods{
						.combat{
							.ether{
								.DMG = etherDmgBuff,
							},
							.exspecial{
								.DMG = skillDmgBuff,
							},
							.ultimate{
								.DMG = skillDmgBuff,
							},
						},
					},
				},
			},
		};
	},
};
