#include "NoblesseOblige.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::noblesseOblige{
	.key{15007},
	.goodKey{"NoblesseOblige"},
	.name = "Noblesse Oblige",
	.setup = []() -> Set::Setup {
		auto fourPcCond = IsActive("noblesseBurst");
		auto fourPcBuff = Requires(fourPcCond, Constant(0.2f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.burst{.DMG = Constant(0.2f)}},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "noblesseBurst",
						.name = "After using an Elemental Burst",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.atk_ = fourPcBuff,
							},
						},
					},
				},
				.mods{
					.teamPreMod{
						.atk_ = fourPcBuff,
					},
				},
			},
		};
	},
};
