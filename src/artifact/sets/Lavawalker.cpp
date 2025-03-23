#include "Lavawalker.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::lavawalker{
	.key{14003},
	.goodKey{"Lavawalker"},
	.name = "Lavawalker",
	.setup = []() -> Set::Setup {
		auto fourPcDMG = Requires(IsActive("lavawalkerCond"), Constant(-0.35f));

		return Set::Setup{
			.twoPc{},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "lavawalkerCond",
						.name = "Opponents affected by Pyro",
						.mods{
							.preMod{
								.all{.DMG = fourPcDMG},
							},
						},
					},
				},
				.mods{
					.preMod{
						.all{.DMG = fourPcDMG},
					},
				},
			},
		};
	},
};
