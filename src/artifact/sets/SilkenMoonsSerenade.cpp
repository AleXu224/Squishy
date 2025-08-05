#include "SilkenMoonsSerenade.hpp"

#include "artifact/setup.hpp"
#include "formula/teamCharacter.hpp"

const Artifact::Set Artifact::Sets::silkenMoonsSerenade{
	.key{15042},
	.goodKey{"SilkenMoonsSerenade"},
	.name = "Silken Moon's Serenade",
	.setup = []() -> Set::Setup {
		auto moonsignLevel = TeamMoonsignLevel{};

		auto cond = IsActive("silkenMoonsSerenadeCond");
		auto buff = Requires{
			cond,
			IfElse{
				.requirement = moonsignLevel >= 2,
				.trueVal = ConstantFlat(60.f),
				.elseVal = Requires{
					.requirement = moonsignLevel >= 1,
					.ret = ConstantFlat(120.f),
				},
			}
		};

		auto condGleamingMoon = IsActive("silkenMoonsSerenadeGleamingMoonStack");
		auto gleamingMoonBuff = Requires{
			condGleamingMoon,
			Constant(0.1f),
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{
						.er = Constant(0.2f),
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "silkenMoonsSerenadeCond",
						.name = "After dealing Elemental DMG",
						.mods{
							.teamPreMod{
								.em = buff,
							},
						},
					},
					Option::Boolean{
						.key = "silkenMoonsSerenadeGleamingMoonStack",
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
					.teamPreMod{
						.em = buff,
						.lunarCharged{.DMG = gleamingMoonBuff},
						.lunarBloom{.DMG = gleamingMoonBuff},
					},
				},
			},
		};
	},
};
