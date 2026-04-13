#include "BloodstainedChivalry.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::bloodstainedChivalry{
	.key{15008},
	.goodKey{"BloodstainedChivalry"},
	.name = "Bloodstained Chivalry",
	.setup = []() -> Set::Setup {
		auto fourPcBuff = Requires{.requirement = IsActive("bloodstainedChivalry"), .ret = Constant{.value = 0.5f}};

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.physical{.DMG = Constant{.value = 0.25f}}},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "bloodstainedChivalry",
						.name = "After defeating an opponent",
						.mods{
							.preMod{
								.charged{.DMG = fourPcBuff},
							},
						},
					},
				},
				.mods{
					.preMod{
						.charged{.DMG = fourPcBuff},
					},
				},
			},
		};
	},
};
