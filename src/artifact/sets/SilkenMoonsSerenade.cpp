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
				.trueVal = ConstantFlat(120.f),
				.elseVal = Requires{
					.requirement = moonsignLevel >= 1,
					.ret = ConstantFlat(60.f),
				},
			}
		};

		auto condGleamingMoon = GetFloat("silkenMoonsSerenadeGleamingMoonEffectCount");
		auto gleamingMoonBuff = condGleamingMoon * Constant(0.1f);

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
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.em = buff,
							},
						},
					},
					Option::ValueList{
						.key = "silkenMoonsSerenadeGleamingMoonEffectCount",
						.prefix = "Gleaming Moon effect count",
						.teamBuff = true,
						.values{1, 2, 3},
						.mods{
							.teamPreMod{
								.allLunar{.DMG = gleamingMoonBuff},
							},
						},
					},
				},
				.mods{
					.teamPreMod{
						.em = buff,
						.allLunar{.DMG = gleamingMoonBuff},
					},
				},
			},
		};
	},
};
