#include "HuskOfOpulentDreams.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::huskOfOpulentDreams{
	.key{15021},
	.goodKey{"HuskOfOpulentDreams"},
	.name = "Husk of Opulent Dreams",
	.setup = []() -> Set::Setup {
		auto fourPcStacks = GetFloat("huskOfOpulentStacks");
		auto fourPcBuffDef = fourPcStacks * 0.06f;
		auto fourPcBuffGeo = fourPcStacks * 0.06f;

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.def_ = Constant(0.3f)},
				},
			},
			.fourPc{
				.opts{
					Option::ValueList{
						.key = "huskOfOpulentStacks",
						.prefix = "Curiosity Stacks",
						.values{1, 2, 3, 4},
						.mods{
							.preMod{
								.def_ = fourPcBuffDef,
								.geo{.DMG = fourPcBuffGeo},
							},
						},
					},
				},
				.mods{
					.preMod{
						.def_ = fourPcBuffDef,
						.geo{.DMG = fourPcBuffGeo},
					},
				},
			},
		};
	},
};
