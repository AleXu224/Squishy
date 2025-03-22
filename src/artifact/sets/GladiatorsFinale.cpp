#include "GladiatorsFinale.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::gladiatorsFinale{
	.key{15001},
	.goodKey{"GladiatorsFinale"},
	.name = "Gladiator's Finale",
	.setup = []() -> Set::Setup {
		auto fourPcCond = IsCharacterWeaponType(Misc::WeaponType::sword) || IsCharacterWeaponType(Misc::WeaponType::claymore) || IsCharacterWeaponType(Misc::WeaponType::polearm);
		auto fourPcBuff = Requires(fourPcCond, Constant(0.35f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.atk_ = Constant(0.18f)},
				},
			},
			.fourPc{
				.opts{},
				.mods{
					.preMod{
						.normal{
							.DMG = fourPcBuff,
						},
					},
				},
			},
		};
	},
};
