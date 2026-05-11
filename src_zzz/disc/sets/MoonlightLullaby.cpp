#include "MoonlightLullaby.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::moonlightLullaby{
	.key{33400},
	.goodKey{"MoonlightLullaby"},
	.name = "Moonlight Lullaby",
	.setup = []() -> Set::Setup {
		auto cond = IsActive("moonlightLullabyCond");
		auto buff = Requires{
			.requirement = IsAgentSpecialty{.specialty = Misc::Specialty::support} && cond,
			.ret = Constant{.value = 0.18f},
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.er = Constant{.value = 0.2f},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "moonlightLullabyCond",
						.name = "After using an EX Special or Ultimate",
						.teamBuff = true,
						.displayCondition = IsAgentSpecialty{.specialty = Misc::Specialty::support},
						.mods{
							.teamCombat{
								.all{
									.DMG = buff,
								},
							},
						},
					},
				},
				.mods{
					.teamCombat{
						.all{
							.DMG = buff,
						},
					},
				},
			},
		};
	},
};
