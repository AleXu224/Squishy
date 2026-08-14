#include "HeartOfTheFurnace.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::heartOfTheFurnace{
	.key{15048},
	.goodKey{"HeartOfTheFurnace"},
	.name = "Heart of the Furnace",
	.setup = []() -> Set::Setup {
		auto cond = IsActive("heartOfTheFurnaceCond");
		auto selfAtkBuff = Requires{
			.requirement = cond,
			.ret = Constant{.value = 0.12f},
		};
		auto teamBuff = Requires{
			.requirement = cond,
			.ret = Constant{.value = 0.5f},
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{
						.atk_ = Constant{.value = 0.18f},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "heartOfTheFurnaceCond",
						.name = "After triggering a Stellar Glimmer reaction or dealing Stellar Glimmer reaction DMG",
						.teamBuff = true,
						.mods{
							.preMod{
								.atk_ = selfAtkBuff,
							},
							.teamPreMod{
								.allStellar{
									.DMG = teamBuff,
								},
							},
						},
					},
				},
				.mods{
					.preMod{
						.atk_ = selfAtkBuff,
					},
					.teamPreMod{
						.allStellar{
							.DMG = teamBuff,
						},
					},
				},
			},
		};
	},
};
