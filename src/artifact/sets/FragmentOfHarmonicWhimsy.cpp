#include "FragmentOfHarmonicWhimsy.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::fragmentOfHarmonicWhimsy{
	.key{15035},
	.goodKey{"FragmentOfHarmonicWhimsy"},
	.name = "Fragment of Harmonic Whimsy",
	.setup = []() -> Set::Setup {
		auto fourPcCond = GetFloat("fragmentOfHarmonicBol");
		auto fourPcBuff = fourPcCond * 0.18f;

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.atk_ = Constant(0.18f)},
				},
			},
			.fourPc{
				.opts{
					Option::ValueList{
						.key = "fragmentOfHarmonicBol",
						.prefix = "The value of a Bond of Life increases or decreases",
						.values{1, 2, 3},
						.mods{
							.preMod{
								.all{
									.DMG = fourPcBuff,
								},
							},
						},
					},
				},
				.mods{
					.preMod{
						.all{
							.DMG = fourPcBuff,
						},
					},
				},
			},
		};
	},
};
