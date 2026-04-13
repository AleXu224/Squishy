#include "GladiatorsFinale.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::gladiatorsFinale{
	.key{15001},
	.goodKey{"GladiatorsFinale"},
	.name = "Gladiator's Finale",
	.setup = []() -> Set::Setup {
		auto fourPcCond = IsCharacterWeaponType{.weaponType = Misc::WeaponType::sword} || IsCharacterWeaponType{.weaponType = Misc::WeaponType::claymore} || IsCharacterWeaponType{.weaponType = Misc::WeaponType::polearm};
		auto fourPcBuff = Requires{.requirement = fourPcCond, .ret = Constant{.value = 0.35f}};

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.atk_ = Constant{.value = 0.18f}},
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
