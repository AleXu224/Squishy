#include "DawnsBloom.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::dawnsBloom{
	.key{33300},
	.goodKey{"DawnsBloom"},
	.name = "Dawn's Bloom",
	.setup = []() -> Set::Setup {
		auto isAttackChar = IsAgentSpecialty{.specialty = Misc::Specialty::attack};
		auto cond = IsActive("dawnsBloomCond");
		auto buff = Requires{
			.requirement = isAttackChar && cond,
			.ret = Constant{.value = 0.2f},
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.basic{
							.DMG = Constant{.value = 0.15f},
						},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "dawnsBloomCond",
						.name = "After using an EX Special Attack or Ultimate",
						.mods{
							.combat{
								.basic{
									.DMG = buff,
								},
							},
						},
					},
				},
				.mods{
					.combat{
						.basic{
							.DMG = Constant{.value = 0.15f} + buff,
						},
					},
				},
			},
		};
	},
};
