#include "ThunderingFury.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::thunderingFury{
	.key{15005},
	.goodKey{"ThunderingFury"},
	.name = "Thundering Fury",
	.setup = []() -> Set::Setup {
		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{
						.electro{.DMG = Constant(0.15f)},
					},
				},
			},
			.fourPc{
				.opts{},
				.mods{
					.preMod{
						.aggravate{.DMG = Constant(0.2f)},
						.superconduct{.DMG = Constant(0.4f)},
						.electroCharged{.DMG = Constant(0.4f)},
						.overloaded{.DMG = Constant(0.4f)},
						.hyperbloom{.DMG = Constant(0.4f)},
					},
				},
			},
		};
	},
};
