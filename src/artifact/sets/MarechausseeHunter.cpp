#include "MarechausseeHunter.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::marechausseeHunter{
	.key{15031},
	.goodKey{"MarechausseeHunter"},
	.name = "Marechaussee Hunter",
	.setup = []() -> Set::Setup {
		auto fourPcCr = GetFloat("marechausseeHpChanged") * 0.12f;

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{
						.normal{
							.DMG = Constant(0.15f),
						},
						.charged{
							.DMG = Constant(0.15f),
						},
					},
				},
			},
			.fourPc{
				.opts{
					Option::ValueList{
						.key = "marechausseeHpChanged",
						.prefix = "Current HP increases or decreases",
						.values{1, 2, 3},
						.mods{
							.preMod{
								.cr = fourPcCr,
							},
						},
					},
				},
				.mods{
					.preMod{
						.cr = fourPcCr,
					},
				},
			},
		};
	},
};
