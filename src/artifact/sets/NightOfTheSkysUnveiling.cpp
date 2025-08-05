#include "NightOfTheSkysUnveiling.hpp"

#include "artifact/setup.hpp"
#include "formula/teamCharacter.hpp"

const Artifact::Set Artifact::Sets::nightOfTheSkysUnveiling{
	.key{15041},
	.goodKey{"NightOfTheSkysUnveiling"},
	.name = "Night of the Sky's Unveiling",
	.setup = []() -> Set::Setup {
		auto moonsignLevel = TeamMoonsignLevel{};

		auto cond = IsActive("nightOfTheSkysUnveilingCond");
		auto buff = Requires{
			cond,
			IfElse{
				.requirement = moonsignLevel >= 2,
				.trueVal = Constant(0.3f),
				.elseVal = Requires{
					.requirement = moonsignLevel >= 1,
					.ret = Constant(0.15f),
				},
			}
		};

		auto condGleamingMoon = IsActive("nightOfTheSkysUnveilingGleamingMoonStack");
		auto gleamingMoonBuff = Requires{
			condGleamingMoon,
			Constant(0.1f),
		};

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
					Option::Boolean{
						.key = "nightOfTheSkysUnveilingCond",
						.name = "When party members trigger Moonglare reactions and the equipping character is on the field",
						.mods{
							.preMod{
								.cr = buff,
							},
						},
					},
					Option::Boolean{
						.key = "nightOfTheSkysUnveilingGleamingMoonStack",
						.name = "Gleaming Moon",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.lunarCharged{.DMG = gleamingMoonBuff},
								.lunarBloom{.DMG = gleamingMoonBuff},
							},
						},
					},
				},
				.mods{
					.preMod{
						.cr = buff,
					},
					.teamPreMod{
						.lunarCharged{.DMG = gleamingMoonBuff},
						.lunarBloom{.DMG = gleamingMoonBuff},
					},
				},
			},
		};
	},
};
