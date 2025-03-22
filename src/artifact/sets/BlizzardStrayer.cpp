#include "BlizzardStrayer.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::blizzardStrayer{
	.key{14001},
	.goodKey{"BlizzardStrayer"},
	.name = "Blizzard Strayer",
	.setup = []() -> Set::Setup {
		auto fourPcCryoCond = IsActive("blizzardStrayerCryo");
		auto fourPcFrozenCond = IsActive("blizzardStrayerFrozen");

		auto fourPcBuffCryo = Requires(fourPcCryoCond, Constant(0.2f));
		auto fourPcBuffFrozen = Requires(fourPcCryoCond && fourPcFrozenCond, Constant(0.2f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.cryo{.DMG = Constant(0.15f)}},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "blizzardStrayerCryo",
						.name = "Opponent affected by Cryo",
						.mods{
							.preMod{
								.cr = fourPcBuffCryo,
							},
						},
					},
					Option::Boolean{
						.key = "blizzardStrayerFrozen",
						.name = "Opponent is Frozen",
						.displayCondition = fourPcCryoCond,
						.mods{
							.preMod{
								.cr = fourPcBuffFrozen,
							},
						},
					},
				},
				.mods{
					.preMod{
						.cr = fourPcBuffCryo + fourPcBuffFrozen,
					},
				},
			},
		};
	},
};
