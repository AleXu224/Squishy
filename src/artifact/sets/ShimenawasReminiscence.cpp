#include "ShimenawasReminiscence.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::shimenawasReminiscence{
	.key{15019},
	.goodKey{"ShimenawasReminiscence"},
	.name = "Shimenawa's Reminiscence",
	.setup = []() -> Set::Setup {
		auto fourPcCond = IsActive("shimenawaUsedEnergy");
		auto fourPcBuff = Requires(fourPcCond, Constant(0.5f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.atk_ = Constant(0.18f)},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "shimenawaUsedEnergy",
						.name = "After casting an Elemental Skill and consuming energy",
						.mods{
							.preMod{
								.normal{.DMG = fourPcBuff},
								.charged{.DMG = fourPcBuff},
								.plunge{.DMG = fourPcBuff},
							},
						},
					},
				},
				.mods{
					.preMod{
						.normal{.DMG = fourPcBuff},
						.charged{.DMG = fourPcBuff},
						.plunge{.DMG = fourPcBuff},
					},
				},
			},
		};
	},
};
