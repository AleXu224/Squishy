#include "DisenchantmentInDeepShadow.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::disenchantmentInDeepShadow{
	.key{15046},
	.goodKey{"DisenchantmentInDeepShadow"},
	.name = "Disenchantment in Deep Shadow",
	.setup = []() -> Set::Setup {
		auto cond = IsActive("disenchantmentInDeepShadow4pcCond");
		auto buff = Requires{
			.requirement = cond,
			.ret = Constant{.value = 0.16f},
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
						.key = "disenchantmentInDeepShadow4pcCond",
						.name = "After the wielder attacks opponents affected by Superconduct or Stellar-Conduct",
						.mods{
							.preMod{
								.cr = buff,
							},
						},
					},
				},
				.mods{
					.preMod{
						.cr = buff,
						.superconduct{
							.DMG = Constant{.value = 0.8f},
						},
						.stellarConduct{
							.DMG = Constant{.value = 0.4f},
						},
					},
				},
			},
		};
	},
};
