#include "GildedDreams.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::gildedDreams{
	.key{15026},
	.goodKey = "GildedDreams",
	.name = "Gilded Dreams",
	.setup = []() -> Set::Setup {
		auto sameElementAtk = 0.14f * SameElementCount{};
		auto otherElementEm = ConstantFlat(50.f) * OtherElementCount{};

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{
						.em = ConstantFlat(80.f),
					},
				},
			},
			.fourPc{
				.opts{},
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