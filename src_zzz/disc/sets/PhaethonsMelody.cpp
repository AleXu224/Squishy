#include "PhaethonsMelody.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::phaethonsMelody{
	.key{33000},
	.goodKey{"PhaethonsMelody"},
	.name = "Phaethon's Melody",
	.setup = []() -> Set::Setup {
		auto condTeam = IsActive("phaetonsMelodyCondTeam");
		auto condSelf = IsActive("phaetonsMelodyCondSelf");

		auto buffTeam = Requires{
			.requirement = condTeam || condSelf,
			.ret = ConstantFlat{.value = 45.f},
		};

		auto buffSelf = Requires{
			.requirement = condSelf,
			.ret = Constant{.value = 0.25f},
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.am_ = Constant{.value = 0.08f},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "phaetonsMelodyCondTeam",
						.name = "Any squad member used an EX Special Attack",
						.teamBuff = true,
						.mods{
							.combat{
								.ap = buffTeam,
							},
						},
					},
					Option::Boolean{
						.key = "phaetonsMelodyCondSelf",
						.name = "Character used an EX Special Attack",
						.teamBuff = false,
						.mods{
							.combat{
								.ether{
									.DMG = buffSelf,
								},
							},
						},
					},
				},
				.mods{
					.combat{
						.ap = buffTeam,
						.ether{
							.DMG = buffSelf,
						},
					},
				},
			},
		};
	},
};
