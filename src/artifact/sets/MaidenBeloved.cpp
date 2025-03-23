#include "MaidenBeloved.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::maidenBeloved{
	.key{14004},
	.goodKey{"MaidenBeloved"},
	.name = "Maiden Beloved",
	.setup = []() -> Set::Setup {
		auto fourPcBuff = Requires(IsActive("maidenBelovedCond"), Constant(0.2f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.hb = Constant(0.15f)},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "maidenBelovedCond",
						.name = "After using an Elemental Skill or Burst",
						.mods{
							.teamPreMod{
								.incHb = fourPcBuff,
							},
						},
					},
				},
				.mods{
					.teamPreMod{
						.incHb = fourPcBuff,
					},
				},
			},
		};
	},
};
