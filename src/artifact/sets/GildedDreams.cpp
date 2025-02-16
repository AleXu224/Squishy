#include "GildedDreams.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::gildedDreams{
	.key{15026},
	.name = "Gilded Dreams",
	.setup = []() -> Set::Setup {
		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{
						.em = ConstantFlat(80.f),
					},
				},
			},
			.fourPc{
				.opts{
					Option::ValueList{
						.key = "gildedSameElement",
						.prefix = "Characters with the same Elemental Type",
						.values = {1, 2, 3},
					},
					Option::ValueList{
						.key = "gildedOtherElement",
						.prefix = "Characters with a different Elemental Type",
						.values = {1, 2, 3},
					},
				},
				.mods{
					.preMod{
						.atk_ = 0.14f * GetFloat("gildedSameElement"),
						.em = ConstantFlat(50.f) * GetFloat("gildedOtherElement"),
					},
				},
			},
		};
	},
};