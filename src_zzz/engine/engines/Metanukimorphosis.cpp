#include "Metanukimorphosis.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::metanukimorphosis{
	.key{14141},
	.goodKey{"Metanukimorphosis"},
	.name = "Metanukimorphosis",
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
		auto multiplier1 = EngineMultiplier(false, {30.f, 34.f, 39.f, 43.f, 48.f});
		auto multiplier2 = EngineMultiplier(false, {60.f, 69.f, 78.f, 87.f, 96.f});

		auto cond = IsActive("metanukimorphosisCond");
		auto buff = Requires{
			.requirement = cond,
			.ret = multiplier1,
		};

		auto cond2 = IsActive("metanukimorphosisCond2");
		auto buff2 = Requires{
			.requirement = cond2,
			.ret = multiplier2,
		};

		return Data::Setup{
			.mods{
				.combat{
					.am = buff,
				},
				.teamCombat{
					.ap = buff2,
				},
			},
			.opts{
				Option::Boolean{
					.key = "metanukimorphosisCond",
					.name = "Equipper's EX Special Attack or Ultimate deals Physical DMG",
					.teamBuff = true,
					.mods{
						.combat{
							.am = buff,
						},
					},
				},
				Option::Boolean{
					.key = "metanukimorphosisCond2",
					.name = "Equipper's Aftershock hits an enemy",
					.teamBuff = true,
					.mods{
						.teamCombat{
							.ap = buff2,
						},
					},
				},
			},
		};
	},
};
