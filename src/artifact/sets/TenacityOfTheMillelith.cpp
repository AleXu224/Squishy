#include "TenacityOfTheMillelith.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::tenacityOfTheMillelith{
	.key{15017},
	.goodKey{"TenacityOfTheMillelith"},
	.name = "Tenacity of the Millelith",
	.setup = []() -> Set::Setup {
		auto fourPcCond = IsActive("tenacitySkillHit");
		auto fourPcAtkBuff = Requires(fourPcCond, Constant(0.2f));
		auto fourPcShieldBuff = Requires(fourPcCond, Constant(0.3f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.hp_ = Constant(0.2f)},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "tenacitySkillHit",
						.name = "Elemental Skill hits an opponent",
						.mods{
							.teamPreMod{
								.atk_ = fourPcAtkBuff,
								.shield_ = fourPcShieldBuff,
							},
						},
					},
				},
				.mods{
					.teamPreMod{
						.atk_ = fourPcAtkBuff,
						.shield_ = fourPcShieldBuff,
					},
				},
			},
		};
	},
};
