#include "FinaleOfTheDeepGalleries.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::finaleOfTheDeepGalleries{
	.key{15040},
	.goodKey{"FinaleOfTheDeepGalleries"},
	.name = "Finale of the Deep Galleries",
	.setup = []() -> Set::Setup {
		auto cond = IsActive("finaleOfTheDeepGalleriesCond");
		auto normalCond = IsActive("finaleOfTheDeepGalleriesNormalCond");
		auto burstCond = IsActive("finaleOfTheDeepGalleriesBurstCond");
		auto normalBuff = Requires(cond && !burstCond, Constant(0.6f));
		auto burstBuff = Requires(cond && !normalCond, Constant(0.6f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{
						.cryo{.DMG = Constant(0.15f)},
						.normal{.DMG = normalBuff},
						.burst{.DMG = burstBuff},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "finaleOfTheDeepGalleriesCond",
						.name = "Character has 0 energy",
						.mods{
							.preMod{
								.normal{.DMG = normalBuff},
								.burst{.DMG = burstBuff},
							},
						},
					},
					Option::Boolean{
						.key = "finaleOfTheDeepGalleriesNormalCond",
						.name = "After the character deals Normal Attack DMG",
						.displayCondition = cond,
					},
					Option::Boolean{
						.key = "finaleOfTheDeepGalleriesBurstCond",
						.name = "After the character deals Elemental Burst DMG",
						.displayCondition = cond,
					},
				},
				.mods{
					.preMod{
						.normal{.DMG = normalBuff},
						.burst{.DMG = burstBuff},
					},
				},
			},
		};
	},
};
