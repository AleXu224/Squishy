#include "PaleFlame.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::paleFlame{
	.key{15018},
	.goodKey{"PaleFlame"},
	.name = "Pale Flame",
	.setup = []() -> Set::Setup {
		auto fourPcAtk = GetFloat("paleFlameStacks") * 0.09f;
		auto fourPcPhys = Requires{.requirement = GetInt("paleFlameStacks") >= 2, .ret = Constant{.value = 0.25f}};

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.physical{.DMG = Constant{.value = 0.25f}}},
				},
			},
			.fourPc{
				.opts{
					Option::ValueList{
						.key = "paleFlameStacks",
						.prefix = "Elemental Skill hits",
						.values{1, 2},
						.mods{
							.preMod{
								.atk_ = fourPcAtk,
								.physical{.DMG = fourPcPhys},
							},
						},
					},
				},
				.mods{
					.preMod{
						.atk_ = fourPcAtk,
						.physical{.DMG = fourPcPhys},
					},
				},
			},
		};
	},
};
