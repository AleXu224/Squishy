#include "GildedDreams.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::gildedDreams{
	.key{15026},
	.name = "Gilded Dreams",
	.setup = []() -> Set::Setup {
		auto sameElementAtk = 0.14f * GetFloat("gildedSameElement");
		auto otherElementEm = ConstantFlat(50.f) * GetFloat("gildedOtherElement");

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
						.teamBuff = true,
						.values = {1, 2, 3},
						.mods{.preMod{.atk_ = sameElementAtk}},
					},
					Option::ValueList{
						.key = "gildedOtherElement",
						.prefix = "Characters with a different Elemental Type",
						.teamBuff = true,
						.values = {1, 2, 3},
						.mods{.preMod{.em = otherElementEm}},
					},
				},
				.mods{
					.preMod{
						.atk_ = sameElementAtk,
						.em = otherElementEm,
					},
				},
			},
		};
	},
};