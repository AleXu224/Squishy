#include "HellfireGears.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::hellfireGears{
	.key{14110},
	.goodKey{"HellfireGears"},
	.name = "Hellfire Gears",
	.baseStats{
		.specialty = Misc::Specialty::stun,
		.rarity = 5,
		.baseAtk = 46,
		.subStat{
			.stat = Stat::impact_,
			.value = 0.072,
		},
	},
	.setup = []() {
		auto multiplier2 = EngineMultiplier(true, {0.1f, 0.125f, 0.15f, 0.175f, 0.2f});

		auto cond = IsActive("hellfireGearsCond");
		auto buff = Requires{
			.requirement = cond,
			.ret = multiplier2,
		};

		return Data::Setup{
			.mods{
				.combat{
					.impact_ = buff,
				},
			},
			.opts{
				Option::Boolean{
					.key = "hellfireGearsCond",
					.name = "After using an EX Special Attack",
					.mods{
						.combat{
							.impact_ = buff,
						},
					},
				},
			},
		};
	},
};
