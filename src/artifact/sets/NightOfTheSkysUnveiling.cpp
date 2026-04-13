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
				.trueVal = Constant{.value = 0.3f},
				.elseVal = Requires{
					.requirement = moonsignLevel >= 1,
					.ret = Constant{.value = 0.15f},
				},
			}
		};

		auto gleamingMoonBuff = Requires{.requirement = cond, .ret = Constant{.value = 0.1f}};

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{
						.em = ConstantFlat{.value = 80.f},
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
							.teamPreMod{
								.allLunar{.DMG = gleamingMoonBuff},
							},
						},
					},
				},
				.mods{
					.preMod{
						.cr = buff,
					},
					.teamPreMod{
						.allLunar{.DMG = gleamingMoonBuff},
					},
				},
			},
		};
	},
};
