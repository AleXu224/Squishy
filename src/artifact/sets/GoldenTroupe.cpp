#include "GoldenTroupe.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::goldenTroupe{
	.key{15032},
	.goodKey = "GoldenTroupe",
	.name = "Golden Troupe",
	.setup = []() -> Set::Setup {
		auto fourPcOffFieldDmg = Requires(
			IsActive("goldenTroupeOffField"),
			Constant(0.25f)
		);

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{
						.skill{
							.DMG = Constant(0.2f),
						},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "goldenTroupeOffField",
						.name = "Character is off field",
						.mods{.preMod{.skill{.DMG = fourPcOffFieldDmg}}},
					},
				},
				.mods{
					.preMod{
						.skill{
							.DMG = Constant(0.25f)
								 + fourPcOffFieldDmg,
						},
					},
				},
			},
		};
	},
};
