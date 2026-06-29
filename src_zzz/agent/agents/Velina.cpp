#include "Velina.hpp"

#include "agent/setup.hpp"

const Agent::Data Agent::Datas::velina{
	.key{1561},
	.factionId = 16,
	.goodKey{"Velina"},
	.name{"Velina"},
	.baseStats{
		.baseHp = 626,
		.baseAtk = 126,
		.baseDef = 49,
		.baseImpact = 86,
		.baseAm = 112,
		.baseAp = 111,
		.coreStat1 = Stat::atk,
		.coreStat2 = Stat::ap,
		.rarity = 5,
		.attribute = Misc::Attribute::wind,
		.specialty = Misc::Specialty::anomaly,
		.hpGrowth = 84.9779,
		.atkGrowth = 7.586,
		.defGrowth = 6.6882,
		.hpUpgrade = {0.000, 430.000, 859.000, 1289.000, 1719.000, 2149.000},
		.atkUpgrade = {0.000, 45.000, 90.000, 134.000, 179.000, 224.000},
		.defUpgrade = {0.000, 34.000, 68.000, 101.000, 135.000, 169.000},
		.coreStat1Upgrade = {0.000, 25.000, 25.000, 50.000, 50.000, 75.000},
		.coreStat2Upgrade = {18.000, 18.000, 36.000, 36.000, 54.000, 54.000},
	},
	.setup = []() -> Data::Setup {
		auto multiplier1 = CoreMultiplier(true, {0.9f, 1.f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f});
		auto multiplier2 = CoreMultiplier(true, {0.85f, 0.95f, 1.05f, 1.15f, 1.25f, 1.35f, 1.45f});
		auto multiplier3 = CoreMultiplier(true, {1.35f, 1.55f, 1.75f, 1.95f, 2.15f, 2.35f, 2.55f});

		auto coreErDmgBuff = Clamp{
			.val1 = (combat.er - 1.2f) * 100.f * 0.0021f,
			.min = 0.f,
			.max = 0.35f,
			.isPercentage = true,
		};
		auto coreErAmBuff = Clamp{
			.val1 = (combat.er - 1.2f) * 100.f * 0.5f,
			.min = 0.f,
			.max = 84.f,
			.isPercentage = true,
		};

		auto coreWindbiteCond = IsActive("velinaCoreWindbiteConsumed");
		auto coreWindbiteBuff = Requires{
			.requirement = IsOriginAgentId{.id = 1561} && OriginAgent{.formula = coreWindbiteCond},
			.ret = Constant{.value = 1.5f},
		};


		auto additionalCond = SpecialtyCountOthers{.specialty = Misc::Specialty::anomaly} >= 1
						   || AttributeCountOthers{.attribute = Misc::Attribute::wind} >= 1;
		auto additionalWindsweptBuff = Requires{
			.requirement = additionalCond,
			.ret = Constant{.value = 0.1f},
		};
		auto additionalVortexBuff = Requires{
			.requirement = additionalCond,
			.ret = Constant{.value = 0.1f},
		};
		auto additionalDazeBuff = Requires{
			.requirement = additionalCond,
			.ret = Constant{.value = 0.3f},
		};

		auto m1DazeBuff = Requires{
			.requirement = Requirement::mindscape1 && additionalCond,
			.ret = Constant{.value = 0.2f},
		};

		auto totalSweepingCycloneDazeBuff = additionalDazeBuff + m1DazeBuff;

		return Data::Setup{
			.mods{
				.combat{
					.am = coreErAmBuff,
					.all{
						.DMG = coreErDmgBuff,
					},
					.windswept{
						.DMG = additionalWindsweptBuff,
					},
					.vortex{
						.DMG = additionalVortexBuff,
					},
				},
				.teamCombat{
					.vortex{
						.additiveMultiplier = coreWindbiteBuff,
					},
				},
			},
			.opts{
				.core{
					Option::Boolean{
						.key = "velinaCoreWindbiteConsumed",
						.name = "After consuming 2 points of Windbite",
						.mods{
							.teamCombat{
								.vortex{
									.additiveMultiplier = coreWindbiteBuff,
								},
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
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.5170f, 0.0470f)
					},
					Node::Atk{
						.name = "2nd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.4890f, 0.0450f)
					},
					Node::Atk{
						.name = "3rd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.6910f, 0.1540f)
					},
					Node::Atk{
						.name = "4th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.2560f, 0.1150f)
					},
					Node::Atk{
						.name = "5th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 2.2960f, 0.2090f)
					},
					Node::Daze{
						.name = "1st-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.2360f, 0.0110f)
					},
					Node::Daze{
						.name = "2nd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.2350f, 0.0110f)
					},
					Node::Daze{
						.name = "3rd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.8050f, 0.0370f)
					},
					Node::Daze{
						.name = "4th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.6290f, 0.0290f)
					},
					Node::Daze{
						.name = "5th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.1270f, 0.0520f)
					},
				},
				.dashAttack{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dashAttack,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 0.9970f, 0.0910f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dashAttack,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 0.4220f, 0.0200f)
					},
				},
				.dodgeCounter{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dodgeCounter,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 3.9490f, 0.3590f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodgeCounter,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 2.8240f, 0.1290f)
					},
				},
				.quickAssist{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::quickAssist,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 2.0380f, 0.1860f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::quickAssist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 0.8620f, 0.0400f)
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
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 5.3270f, 0.4850f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::assistFollowup,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 3.9670f, 0.1810f)
					},
				},
				.special{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 0.5860f, 0.0540f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.4960f, 0.0230f)
					},
				},
				.exspecial{
					Node::Atk{
						.name = "Wind Shear - Purifying Rise DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 4.4050f, 0.4010f)
					},
					Node::Daze{
						.name = "Wind Shear - Purifying Rise Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 3.0250f, 0.1380f)
					},
					Node::Info{
						.name = "Wind Shear - Purifying Rise Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 45.f, 0.f)
					},
					Node::Atk{
						.name = "Wind Shear - Triple Deathblow DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 7.2520f, 0.6600f)
					},
					Node::Daze{
						.name = "Wind Shear - Triple Deathblow Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 5.1520f, 0.2350f)
					},
					Node::Info{
						.name = "Wind Shear - Triple Deathblow Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 45.f, 0.f)
					},
					Node::Atk{
						.name = "Wind Shear - Eye of the Storm DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 2.1670f, 0.1970f)
					},
					Node::Daze{
						.name = "Wind Shear - Eye of the Storm Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 1.8340f, 0.0840f)
					},
					Node::Info{
						.name = "Wind Shear - Eye of the Storm Windbloom Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 90.f, 0.f)
					},
					Node::Atk{
						.name = "Sweeping Cyclone Wind Attribute Single-Hit DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 0.3900f, 0.0360f)
					},
					Node::Daze{
						.name = "Sweeping Cyclone Wind Attribute Single-Hit Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.8250f, 0.0380f),
						.modifier{
							.enemy{
								.daze = totalSweepingCycloneDazeBuff,
							},
						},
					},
					Node::Atk{
						.name = "Sweeping Cyclone Attribute-Converted Single-Hit DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 0.9750f, 0.0890f)
					},
					Node::Atk{
						.name = "Condensed Cyclone DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 0.3250f, 0.0300f)
					},
					Node::Daze{
						.name = "Condensed Cyclone Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 1.3750f, 0.0630f)
					},
				},
				.chain{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 8.0040f, 0.7280f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 2.3840f, 0.1090f)
					},
				},
				.ultimate{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::ultimate,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 21.8830f, 1.9900f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::ultimate,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 3.8130f, 0.1740f)
					},
				},
				.core = [&]() {
					std::vector<Node::Types> ret{
						Node::Mods{
							.mods{
								.combat{
									.am = coreErAmBuff,
									.all{
										.DMG = coreErDmgBuff,
									},
									.windswept{
										.DMG = additionalWindsweptBuff,
									},
									.vortex{
										.DMG = additionalVortexBuff,
									},
								},
							},
						},
						Node::Info{
							.name = "Sweeping Cyclone Daze Increase",
							.type = Utils::EntryType::multiplier,
							.formula = totalSweepingCycloneDazeBuff,
						},
					};
					addAblooms(
						std::array<FloatNode, 6>{
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 1.45f},
						},
						ret,
						"Condensed Cyclone Abloom"
					);
					addAblooms(
						std::array<FloatNode, 6>{
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 2.55f},
						},
						ret,
						"Sweeping Cyclone Abloom"
					);
					addAblooms(
						std::array<FloatNode, 6>{
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 0.f},
							ConstantFlat{.value = 6.8f},
						},
						ret,
						"Ultimate Abloom"
					);
					return ret;
				}(),
			},
		};
	},
};
