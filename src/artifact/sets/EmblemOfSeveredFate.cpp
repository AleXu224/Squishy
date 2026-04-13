#include "EmblemOfSeveredFate.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::emblemOfSeveredFate{
	.key{15020},
	.goodKey{"EmblemOfSeveredFate"},
	.name = "Emblem of Severed Fate",
	.setup = []() -> Set::Setup {
		auto fourPcBuff = Min{.val1 = total.er * 0.25f, .val2 = Constant{.value = 0.75f}};
		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.er = Constant{.value = 0.2f}},
				},
			},
			.fourPc{
				.opts{},
				.mods{
					.preMod{
						.burst{
							.DMG = fourPcBuff,
						},
					},
				},
			},
		};
	},
};
