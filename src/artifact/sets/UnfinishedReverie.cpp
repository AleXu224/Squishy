#include "UnfinishedReverie.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::unfinishedReverie{
	.key{15036},
	.goodKey{"UnfinishedReverie"},
	.name = "Unfinished Reverie",
	.setup = []() -> Set::Setup {
		auto fourPcStacks = GetFloat("unfinishedReverieStacks");
		auto fourPcBuff = fourPcStacks * 0.1f;

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.atk_ = Constant(0.18f)},
				},
			},
			.fourPc{
				.opts{
					Option::ValueList{
						.key = "unfinishedReverieStacks",
						.prefix = "Stacks",
						.values{1, 2, 3, 4, 5},
						.mods{
							.preMod{
								.all{
									.DMG = fourPcBuff,
								},
							},
						},
					},
				},
				.mods{
					.preMod{
						.all{
							.DMG = fourPcBuff,
						},
					},
				},
			},
		};
	},
};
