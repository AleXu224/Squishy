#include "MistsplitterReforged.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::mistsplitterReforged{
	.key{11509},
	.goodKey{"MistsplitterReforged"},
	.name = "Mistsplitter Reforged",
	.baseStats{
		.type = Misc::WeaponType::sword,
		.rarity = 5,
		.baseAtk = 47.537,
		.atkCurve = Curves::WeaponGrow::ATTACK_302,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cd,
				.value = 0.096,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.1200, 0.1500, 0.1800, 0.2100, 0.2400});
		auto multiplier2 = WeaponMultiplier(true, {0.0800, 0.1000, 0.1200, 0.1400, 0.1600});
		auto multiplier3 = WeaponMultiplier(true, {0.1600, 0.2000, 0.2400, 0.2800, 0.3200});
		auto multiplier4 = WeaponMultiplier(true, {0.2800, 0.3500, 0.4200, 0.4900, 0.5600});

		auto condVal = GetIndex("mistsplitterCond");
		auto cond = IsActive("mistsplitterCond");

		auto buff = Requires{
			.requirement = cond,
			.ret = Evaluator{
				.evaluated = Index{
					.index = condVal,
					.indexable = std::array{multiplier2, multiplier3, multiplier4},
				},
				.isPercentage = true
			},
		};

		auto pyroBuff = Requires{.requirement = IsCharacterElement{.element = Misc::Element::pyro}, .ret = buff};
		auto hydroBuff = Requires{.requirement = IsCharacterElement{.element = Misc::Element::hydro}, .ret = buff};
		auto cryoBuff = Requires{.requirement = IsCharacterElement{.element = Misc::Element::cryo}, .ret = buff};
		auto electroBuff = Requires{.requirement = IsCharacterElement{.element = Misc::Element::electro}, .ret = buff};
		auto dendroBuff = Requires{.requirement = IsCharacterElement{.element = Misc::Element::dendro}, .ret = buff};
		auto anemoBuff = Requires{.requirement = IsCharacterElement{.element = Misc::Element::anemo}, .ret = buff};
		auto geoBuff = Requires{.requirement = IsCharacterElement{.element = Misc::Element::geo}, .ret = buff};
		return Data::Setup{
			.mods{
				.preMod{
					.pyro{.DMG = pyroBuff},
					.hydro{.DMG = hydroBuff},
					.cryo{.DMG = cryoBuff},
					.electro{.DMG = electroBuff},
					.dendro{.DMG = dendroBuff},
					.anemo{.DMG = anemoBuff},
					.geo{.DMG = geoBuff},
					.allElemental{.DMG = multiplier1},
				},
			},
			.opts{
				Option::ValueList{
					.key = "mistsplitterCond",
					.prefix = "Stacks",
					.values{1, 2, 3},
					.mods{
						.preMod{
							.pyro{.DMG = pyroBuff},
							.hydro{.DMG = hydroBuff},
							.cryo{.DMG = cryoBuff},
							.electro{.DMG = electroBuff},
							.dendro{.DMG = dendroBuff},
							.anemo{.DMG = anemoBuff},
							.geo{.DMG = geoBuff},
						},
					},
				},
			},
		};
	},
};
