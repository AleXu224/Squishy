#include "Norma.hpp"

#include "agent/setup.hpp"

const Agent::Data Agent::Datas::norma{
	.key{1571},
	.factionId = 16,
	.goodKey{"Norma"},
	.name{"Norma"},
	.baseStats{
		.baseHp = 627,
		.baseAtk = 110,
		.baseDef = 47,
		.baseImpact = 106,
		.baseAm = 99,
		.baseAp = 98,
		.coreStat1 = Stat::atk,
		.coreStat2 = Stat::cr,
		.rarity = 5,
		.attribute = Misc::Attribute::fire,
		.specialty = Misc::Specialty::stun,
		.hpGrowth = 85.1001,
		.atkGrowth = 6.4646,
		.defGrowth = 6.3487,
		.hpUpgrade = {0.000, 430.000, 861.000, 1291.000, 1721.000, 2152.000},
		.atkUpgrade = {0.000, 39.000, 78.000, 117.000, 156.000, 196.000},
		.defUpgrade = {0.000, 32.000, 64.000, 96.000, 128.000, 161.000},
		.coreStat1Upgrade = {0.000, 25.000, 25.000, 50.000, 50.000, 75.000},
		.coreStat2Upgrade = {0.048, 0.048, 0.096, 0.096, 0.144, 0.144},
	},
	.setup = []() -> Data::Setup {
		auto multiplier1 = CoreMultiplier(true, {0.0086f, 0.01f, 0.011399999999999999f, 0.0128f, 0.014199999999999999f, 0.015600000000000001f, 0.017f});
		auto multiplier2 = CoreMultiplier(true, {0.43f, 0.5f, 0.57f, 0.64f, 0.71f, 0.78f, 0.85f});
		auto multiplier3 = CoreMultiplier(true, {0.0044f, 0.005f, 0.005600000000000001f, 0.0062f, 0.0068000000000000005f, 0.0074f, 0.008f});
		auto multiplier4 = CoreMultiplier(true, {0.22f, 0.25f, 0.28f, 0.31f, 0.34f, 0.37f, 0.4f});

		auto coreCdBuff = Min{
			.val1 = (initial.cr - Constant{.value = 0.5f}) * ConstantFlat{.value = 100.f} * multiplier1,
			.val2 = multiplier2,
		};
		auto coreDazeBuff = Min{
			.val1 = (initial.cr - Constant{.value = 0.5f}) * ConstantFlat{.value = 100.f} * multiplier3,
			.val2 = multiplier4,
		};

		auto coreAtkBuff = Min{.val1 = combat.sheerForce * Constant{.value = 1.25f}, .val2 = ConstantFlat{.value = 1200.f}};

		auto additionalCond = SpecialtyCountOthers{.specialty = Misc::Specialty::attack} >= 1.f || SpecialtyCountOthers{.specialty = Misc::Specialty::rupture} >= 1.f;
		auto techDivideStacks = GetFloat("techDivideStacks");
		auto techDivideStackVal = IfElse{
			.requirement = Requirement::mindscape2,
			.trueVal = Constant{.value = 0.06f},
			.elseVal = Constant{.value = 0.03f},
		};
		auto techDivideStunMult = Requires{
			.requirement = additionalCond,
			.ret = techDivideStacks * techDivideStackVal,
		};

		auto additionalAtkBuff = Requires{
			.requirement = additionalCond && IsActive("ehNahBarrageCond"),
			.ret = Min{.val1 = ConstantFlat{.value = 44.f} + agent.level * 14, .val2 = ConstantFlat{.value = 870.f}},
		};

		auto m1Cond = IsActive("m1Cond");
		auto m1Buff = Requires{
			.requirement = m1Cond && Requirement::mindscape1,
			.ret = Constant{.value = -0.15f},
		};

		Stats::ModsSheet::_EnemySheet::_SkillValue resSheet{};
		for (const auto &attribute: Misc::attributes) {
			Stats::fromEnemyResAttributeDirect(resSheet, attribute) = m1Buff;
		}

		auto m6Daze = Requires{
			.requirement = Requirement::mindscape6,
			.ret = Constant{.value = 0.3f},
		};
		auto m6Dmg = Requires{
			.requirement = Requirement::mindscape6,
			.ret = Constant{.value = 0.3f},
		};

		return Data::Setup{
			.mods{
				.combat{
					.atk = coreAtkBuff + additionalAtkBuff,
					.cd = coreCdBuff,
					.special{.enemy{.daze = coreDazeBuff}},
					.exspecial{.enemy{.daze = coreDazeBuff}},
					.ultimate{.enemy{.daze = coreDazeBuff}},
				},
				.enemy{
					.stunMod = techDivideStunMult,
					.resistance = resSheet,
				},
			},
			.opts{
				.core{
					Option::ValueSlider{
						.key = "techDivideStacks",
						.name = "After Norma, her creations, or Combat Bangboo hit an enemy",
						.teamBuff = true,
						.values = std::views::iota(0, 11) | std::ranges::to<std::vector<float>>(),
						.mods{
							.enemy{
								.stunMod = techDivideStunMult,
							},
						},
					},
					Option::Boolean{
						.key = "ehNahBarrageCond",
						.name = "En-Nah Barrage is active",
						.mods{
							.combat{
								.atk = additionalAtkBuff,
							},
						},
					},
				},
				.mindscape1{
					Option::Boolean{
						.key = "m1Cond",
						.name = "After an Armor-Piercing Warhead or High-Explosive Warhead from any skill hits an enemy",
						.teamBuff = true,
						.mods{
							.enemy{
								.resistance = resSheet,
							},
						},
					},
				},
			},
			.nodes{
				.basic{
					Node::Atk{
						.name = "1st-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.2610f, 0.0240f)
					},
					Node::Atk{
						.name = "2nd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.3780f, 0.0350f)
					},
					Node::Atk{
						.name = "3rd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.7330f, 0.0670f)
					},
					Node::Atk{
						.name = "4th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.4090f, 0.1290f)
					},
					Node::Daze{
						.name = "1st-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.1560f, 0.0080f)
					},
					Node::Daze{
						.name = "2nd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.2030f, 0.0100f)
					},
					Node::Daze{
						.name = "3rd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.3780f, 0.0180f)
					},
					Node::Daze{
						.name = "4th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.8830f, 0.0410f)
					},
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.5690f, 0.0520f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.3400f, 0.0160f)
					},
				},
				.dashAttack{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 0.8230f, 0.0750f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 0.3490f, 0.0160f)
					},
				},
				.dodgeCounter{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dodgeCounter,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 3.1310f, 0.2850f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodgeCounter,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 2.2920f, 0.1050f)
					},
				},
				.quickAssist{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 0.7050f, 0.0650f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 0.5960f, 0.0280f)
					},
				},
				.defensiveAssist{
					Node::Daze{
						.name = "Light Defensive Daze",
						.source = Misc::AttackSource::defensiveAssist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 2.7130f, 0.1240f)
					},
					Node::Daze{
						.name = "Heavy Defensive Daze",
						.source = Misc::AttackSource::defensiveAssist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 3.4280f, 0.1560f)
					},
					Node::Daze{
						.name = "Chain Defensive Daze",
						.source = Misc::AttackSource::defensiveAssist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 1.6680f, 0.0760f)
					},
				},
				.assistFollowup{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::assistFollowup,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 4.5080f, 0.4100f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::assistFollowup,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 3.3240f, 0.1520f)
					},
				},
				.special{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 0.1500f, 0.0140f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.1690f, 0.0080f)
					},
					Node::Atk{
						.name = "Target Practice DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 1.9500f, 0.1780f)
					},
					Node::Daze{
						.name = "Target Practice Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 1.5130f, 0.0690f)
					},
					Node::Info{
						.name = "Target Practice Auto-Fire Interval",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableSkill::special, 3.f, 0.f)
					},
				},
				.exspecial{
					Node::Atk{
						.name = "Shot DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 2.0540f, 0.1870f)
					},
					Node::Daze{
						.name = "Shot Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 1.4390f, 0.0660f)
					},
					Node::Atk{
						.name = "Armor-Piercing Warhead DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 3.0800f, 0.2800f)
					},
					Node::Daze{
						.name = "Armor-Piercing Warhead Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 2.1580f, 0.0990f),
						.modifier{
							.enemy{
								.daze = m6Daze,
							},
						},
					},
					Node::Atk{
						.name = "High-Explosive Warhead DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 3.4140f, 0.3110f),
						.modifier{
							.DMG = m6Dmg,
						},
					},
					Node::Daze{
						.name = "High-Explosive Warhead Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 1.7270f, 0.0790f)
					},
					Node::Info{
						.name = "Energy Cost to Use",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 40.f, 0.f)
					},
					Node::Atk{
						.name = "Extended Shot DMG Per Sec",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 1.3060f, 0.1190f)
					},
					Node::Daze{
						.name = "Extended Shot Daze Per Sec",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.9390f, 0.0430f)
					},
					Node::Atk{
						.name = "Extended Shot Armor-Piercing Warhead DMG Per Sec",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 1.9590f, 0.1790f)
					},
					Node::Daze{
						.name = "Extended Shot Armor-Piercing Warhead Daze Per Sec",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 1.4090f, 0.0650f),
						.modifier{
							.enemy{
								.daze = m6Daze,
							},
						},
					},
					Node::Atk{
						.name = "Extended Shot High-Explosive Warhead DMG Per Sec",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 2.1670f, 0.1970f),
						.modifier{
							.DMG = m6Dmg,
						},
					},
					Node::Daze{
						.name = "Extended Shot High-Explosive Warhead Daze Per Sec",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 1.1270f, 0.0520f)
					},
					Node::Info{
						.name = "Extended Shot Energy Cost Per Sec",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 20.f, 0.f)
					},
					Node::Atk{
						.name = "Additional Armor-Piercing Warhead DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 0.8280f, 0.0760f)
					},
					Node::Daze{
						.name = "Additional Armor-Piercing Warhead Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.3500f, 0.0160f),
						.modifier{
							.enemy{
								.daze = m6Daze,
							},
						},
					},
					Node::Atk{
						.name = "Additional High-Explosive Warhead DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 0.9230f, 0.0840f),
						.modifier{
							.DMG = m6Dmg,
						},
					},
					Node::Daze{
						.name = "Additional High-Explosive Warhead Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.2800f, 0.0130f)
					},
					Node::Info{
						.name = "Enhanced Form Auto-Fire Interval",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableSkill::special, 2.f, 0.f)
					},
				},
				.chain{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 7.5270f, 0.6850f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 1.9800f, 0.0900f)
					},
				},
				.ultimate{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::ultimate,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 19.3650f, 1.7610f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::ultimate,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 7.9250f, 0.3610f)
					},
				},
				.core{
					Node::Mods{
						.mods{
							.combat{
								.atk = coreAtkBuff,
								.cd = coreCdBuff,
								.special{.enemy{.daze = coreDazeBuff}},
								.exspecial{.enemy{.daze = coreDazeBuff}},
								.ultimate{.enemy{.daze = coreDazeBuff}},
							},
						},
					},
				},
				.mindscape2{
					Node::Atk{
						.name = "Missile DMG",
						.source = Misc::AttackSource::ultimate,
						.formula = combat.atk * 2.f,
					},
				},
			},
		};
	},
};
