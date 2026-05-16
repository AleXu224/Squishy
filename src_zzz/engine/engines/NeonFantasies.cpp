#include "NeonFantasies.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::neonFantasies{
	.key{14151},
	.goodKey{"NeonFantasies"},
	.name = "Neon Fantasies",
	.baseStats{
		.specialty = Misc::Specialty::stun,
		.rarity = 5,
		.baseAtk = 48,
		.subStat{
			.stat = Stat::am_,
			.value = 0.120,
		},
	},
	.setup = []() {
		auto multiplier1 = EngineMultiplier(false, {90.f, 103.f, 117.f, 130.f, 145.f});
		auto multiplier2 = EngineMultiplier(true, {0.15f, 0.17f, 0.195f, 0.21f, 0.24f});
		auto multiplier3 = EngineMultiplier(false, {60.f, 69.f, 78.f, 87.f, 96.f});

		auto stacks = GetInt("neonFantasiesStacks");
		auto buff = multiplier2 * stacks;
		auto selfBuff = Requires{
			.requirement = stacks >= 2,
			.ret = multiplier3,
		};

		return Data::Setup{
			.mods{
				.combat{
					.ap = multiplier1 + selfBuff,
				},
				.teamCombat{
					.all{
						.DMG = buff,
					},
				},
			},
			.opts{
				Option::ValueList{
					.key = "neonFantasiesStacks",
					.prefix = "After the equipper's EX Special Attack or Basic Attack deals Ether DMG",
					.values = std::views::iota(0, 3) | std::ranges::to<std::vector<uint32_t>>(),
					.mods{
						.combat{
							.ap = selfBuff,
						},
						.teamCombat{
							.all{
								.DMG = buff,
							},
						},
					},
				},
			},
		};
	},
};
