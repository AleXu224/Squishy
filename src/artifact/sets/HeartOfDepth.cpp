#include "HeartOfDepth.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::heartOfDepth{
	.key{15016},
	.goodKey{"HeartOfDepth"},
	.name = "Heart of Depth",
	.setup = []() -> Set::Setup {
		auto fourPcCond = IsActive("heartOfDepthSkill");
		auto fourPcBuff = Requires(fourPcCond, Constant(0.3f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.hydro{.DMG = Constant(0.15f)}},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "heartOfDepthSkill",
						.name = "After using Elemental Skill",
						.mods{
							.preMod{
								.normal{.DMG = fourPcBuff},
								.charged{.DMG = fourPcBuff},
							},
						},
					},
				},
				.mods{
					.preMod{
						.normal{.DMG = fourPcBuff},
						.charged{.DMG = fourPcBuff},
					},
				},
			},
		};
	},
};
