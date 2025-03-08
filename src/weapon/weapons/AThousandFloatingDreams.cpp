#include "AThousandFloatingDreams.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::aThousandFloatingDreams{
	.key{14511},
	.goodKey = "AThousandFloatingDreams",
	.name = "A Thousand Floating Dreams",
	.baseStats{
		.type = Misc::WeaponType::catalyst,
		.rarity = 5,
		.baseAtk = 44.336,
		.atkCurve = Curves::WeaponGrow::ATTACK_304,
		.subStat = SubStat{
			.stat{
				.stat = Stat::em,
				.value = 57.600,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(false, {32.0000, 40.0000, 48.0000, 56.0000, 64.0000});
		auto multiplier2 = WeaponMultiplier(true, {0.1000, 0.1400, 0.1800, 0.2200, 0.2600});
		auto multiplier3 = WeaponMultiplier(false, {40.0000, 42.0000, 44.0000, 46.0000, 48.0000});

		auto sameElementCount = Min(SameElementCount{}, 3);
		auto otherElementCount = Min(OtherElementCount{}, 3);

		auto emBuff = sameElementCount * multiplier1;
		auto dmgBuff = otherElementCount * multiplier2;
		auto dmgBuffPyro = Requires(IsCharacterElement(Misc::Element::pyro), dmgBuff);
		auto dmgBuffHydro = Requires(IsCharacterElement(Misc::Element::hydro), dmgBuff);
		auto dmgBuffCryo = Requires(IsCharacterElement(Misc::Element::cryo), dmgBuff);
		auto dmgBuffElectro = Requires(IsCharacterElement(Misc::Element::electro), dmgBuff);
		auto dmgBuffDendro = Requires(IsCharacterElement(Misc::Element::dendro), dmgBuff);
		auto dmgBuffAnemo = Requires(IsCharacterElement(Misc::Element::anemo), dmgBuff);
		auto dmgBuffGeo = Requires(IsCharacterElement(Misc::Element::geo), dmgBuff);

		return Data::Setup{
			.mods{
				.preMod{
					.em = emBuff - multiplier3,
					.pyro{.DMG = dmgBuffPyro},
					.hydro{.DMG = dmgBuffHydro},
					.cryo{.DMG = dmgBuffCryo},
					.electro{.DMG = dmgBuffElectro},
					.dendro{.DMG = dmgBuffDendro},
					.anemo{.DMG = dmgBuffAnemo},
					.geo{.DMG = dmgBuffGeo},
				},
				.teamPreMod{
					.em = multiplier3,
				},
			},
		};
	},
};
