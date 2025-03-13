#include "CrimsonWitchOfFlames.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::crimsonWitchOfFlames{
	.key{15006},
	.goodKey{"CrimsonWitchOfFlames"},
	.name = "Crimson Witch of Flames",
	.setup = []() -> Set::Setup {
		auto fourPcDMG = Requires(IsActive("crimsonWitchSkill"), Constant(0.075f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{
						.pyro{
							.DMG = Constant(0.15f),
						},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "crimsonWitchSkill",
						.name = "After using the Elemental Skill",
						.mods{
							.preMod{
								.pyro{.DMG = fourPcDMG},
							},
						},
					},
				},
				.mods{
					.preMod{
						.pyro{.DMG = fourPcDMG},
						.vape{.DMG = Constant(0.15f)},
						.melt{.DMG = Constant(0.15f)},
						.burning{.DMG = Constant(0.4f)},
						.overloaded{.DMG = Constant(0.4f)},
						.burgeon{.DMG = Constant(0.4f)},
					},
				},
			},
		};
	},
};
