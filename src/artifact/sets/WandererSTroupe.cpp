#include "WanderersTroupe.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::wanderersTroupe{
	.key{15003},
	.goodKey{"WanderersTroupe"},
	.name = "Wanderer's Troupe",
	.setup = []() -> Set::Setup {
		auto fourPcCond = IsCharacterWeaponType(Misc::WeaponType::catalyst) || IsCharacterWeaponType(Misc::WeaponType::bow);
		auto fourPcBuff = Requires(fourPcCond, Constant(0.35f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.em = ConstantFlat(80.f)},
				},
			},
			.fourPc{
				.opts{},
				.mods{
					.preMod{
						.charged{
							.DMG = fourPcBuff,
						},
					},
				},
			},
		};
	},
};
