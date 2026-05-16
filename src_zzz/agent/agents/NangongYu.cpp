#include "NangongYu.hpp"

#include "agent/setup.hpp"

const Agent::Data Agent::Datas::nangongYu{
	.key{1511},
	.factionId = 13,
	.goodKey{"NangongYu"},
	.name{"Nangong Yu"},
	.baseStats{
		.baseHp = 673,
		.baseAtk = 107,
		.baseDef = 50,
		.baseImpact = 120,
		.baseAm = 90,
		.baseAp = 93,
		.coreStat1 = Stat::atk,
		.coreStat2 = Stat::am,
		.rarity = 5,
		.attribute = Misc::Attribute::ether,
		.specialty = Misc::Specialty::stun,
		.hpGrowth = 91.3706,
		.atkGrowth = 6.3136,
		.defGrowth = 6.7901,
		.hpUpgrade = {0.000, 462.000, 924.000, 1386.000, 1848.000, 2310.000},
		.atkUpgrade = {0.000, 38.000, 77.000, 115.000, 153.000, 192.000},
		.defUpgrade = {0.000, 34.000, 69.000, 103.000, 137.000, 172.000},
		.coreStat1Upgrade = {0.000, 25.000, 25.000, 50.000, 50.000, 75.000},
		.coreStat2Upgrade = {12.000, 12.000, 24.000, 24.000, 36.000, 36.000},
	},
	.setup = []() -> Data::Setup {
		auto multiplier1 = CoreMultiplier(false, {60.f, 70.f, 80.f, 90.f, 100.f, 110.f, 120.f});
		auto multiplier3 = CoreMultiplier(true, {0.11f, 0.125f, 0.14f, 0.155f, 0.17f, 0.185f, 0.2f});
		auto multiplier4 = CoreMultiplier(true, {0.13f, 0.15f, 0.17f, 0.19f, 0.21f, 0.23f, 0.25f});

		auto coreImpactBuff = Max{.val1 = initial.am - 110.f, .val2 = ConstantFlat{.value = 0.f}};

		auto vibratoStacks = GetFloat("nangongVibrato");
		auto abloomAdditionalMult = Requires{
			.requirement = Requirement::enemyStunned,
			.ret = vibratoStacks
				 * (0.25f + Requires{
						.requirement = Requirement::mindscape2,
						.ret = Constant{.value = 0.1f},
					}),
		};

		auto coreBasicOrExHitCond = IsActive("nangongBasicOrExHit");
		auto coreBasicOrExHitDazeIncrease = Requires{
			.requirement = coreBasicOrExHitCond && IsOriginAgentId{.id = 1511},
			.ret = multiplier3,
		};
		auto coreBasicOrExHitBuff = Requires{
			.requirement = coreBasicOrExHitCond,
			.ret = multiplier4,
		};

		auto coreMisstepCond = IsActive("nangongMisstep");
		auto coreMisstepBuff = Requires{
			.requirement = coreMisstepCond && (SpecialtyCountOthers{.specialty = Misc::Specialty::anomaly} >= 1 || FactionCountOthers{.factionId = 13} >= 1),
			.ret = Constant{.value = 0.3f}
				 + Requires{
					 .requirement = Requirement::mindscape2,
					 .ret = Constant{.value = 0.3f},
				 },
		};

		auto basicPreciseChargingCond = IsActive("nangongPreciseCharging");
		auto basicPreciseChargingBuff = Requires{
			.requirement = basicPreciseChargingCond,
			.ret = Constant{.value = 0.2f},
		};

		auto ultimateEtherVeilCond = IsActive("nangongEtherVeil");
		auto ultimateEtherVeilBuff = Requires{
			.requirement = ultimateEtherVeilCond,
			.ret = ConstantFlat{.value = 50.f},
		};

		auto m1Cond = IsActive("nangongMindscape1");
		auto m1Buff = Requires{
			.requirement = Requirement::mindscape1 && m1Cond,
			.ret = Constant{.value = -0.18f},
		};

		auto m4Buff = Requires{
			.requirement = Requirement::mindscape4,
			.ret = ConstantFlat{.value = 40.f},
		};

		auto m6DazeBuff = Requires{
			.requirement = Requirement::mindscape6,
			.ret = Constant{.value = 0.5f},
		};
		auto m6VibratoStacks = GetFloat("nangongM6Vibrato");
		auto m6VibratoBuff = Requires{
			.requirement = Requirement::mindscape6,
			.ret = m6VibratoStacks * 0.25f,
		};

		return Data::Setup{
			.mods{
				.combat{
					.ap = multiplier1 + m4Buff,
					.impact = coreImpactBuff,
					.all{
						.daze = m6DazeBuff,
					},
					.basic{
						.daze = basicPreciseChargingBuff,
					},
				},
				.teamCombat{
					.atk = ultimateEtherVeilBuff,
					.all{
						.DMG = coreBasicOrExHitBuff,
					},
				},
				.enemy{
					.stunMod = coreMisstepBuff,
					.daze = coreBasicOrExHitDazeIncrease,
					.resistance{
						.physical = m1Buff,
						.fire = m1Buff,
						.ice = m1Buff,
						.electric = m1Buff,
						.ether = m1Buff,
					},
				},
			},
			.opts{
				.basic{
					Option::Boolean{
						.key = "nangongPreciseCharging",
						.name = "After precise charging",
						.mods{
							.combat{
								.basic{
									.daze = basicPreciseChargingBuff,
								},
							},
						},
					},
				},
				.ultimate{
					Option::Boolean{
						.key = "nangongEtherVeil",
						.name = "Ether Veil active",
						.mods{
							.teamCombat{
								.atk = ultimateEtherVeilBuff,
							},
						},
					},
				},
				.core{
					Option::ValueSlider{
						.key = "nangongVibrato",
						.name = "Vibrato Stacks",
						.displayCondition = Requirement::enemyStunned,
						.values = std::views::iota(0, 5) | std::ranges::to<std::vector<float>>(),
						.nodes = {
							Node::Info{
								.name = "Abloom DMG ratio",
								.type = Utils::EntryType::multiplier,
								.formula = abloomAdditionalMult,
							},
						},
					},
					Option::Boolean{
						.key = "nangongBasicOrExHit",
						.name = "After Basic Attack: Adorable Explosive Impact or EX Special Attack: The Unbearable Weight of Love hits an enemy",
						.teamBuff = true,
						.mods{
							.teamCombat{
								.all{
									.DMG = coreBasicOrExHitBuff,
								},
							},
							.enemy{
								.daze = coreBasicOrExHitDazeIncrease,
							},
						},
					},
					Option::Boolean{
						.key = "nangongMisstep",
						.name = "After the third hit of Basic Attack: Adorable Explosive Impact or Ultimate: Meteor Shower hits an enemy",
						.mods{
							.enemy{
								.stunMod = coreMisstepBuff,
							},
						},
					},
				},
				.mindscape1{
					Option::Boolean{
						.key = "nangongMindscape1",
						.name = "After EX Special Attack or Basic Attack: Adorable Explosive Impact hits an enemy",
						.mods{
							.enemy{
								.resistance{
									.physical = m1Buff,
									.fire = m1Buff,
									.ice = m1Buff,
									.electric = m1Buff,
									.ether = m1Buff,
								},
							},
						},
					},
				},
				.mindscape6{
					Option::ValueSlider{
						.key = "nangongM6Vibrato",
						.name = "Vibrato: Modified Stacks",
						.values = std::views::iota(0, 5) | std::ranges::to<std::vector<float>>(),
						.nodes = {
							Node::Info{
								.name = "Vibrato: Modified Abloom DMG ratio",
								.type = Utils::EntryType::multiplier,
								.formula = m6VibratoBuff,
							},
						},
					},
				},
			},
			.nodes{
				.basic{
					Node::Atk{.name = "Shooting Star Step 1st-Hit DMG", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.atk, LevelableSkill::basic, 1.1490f, 0.1050f)},
					Node::Atk{.name = "Shooting Star Step 2nd-Hit DMG", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.atk, LevelableSkill::basic, 1.3690f, 0.1250f)},
					Node::Atk{.name = "Shooting Star Step 3rd-Hit DMG", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.atk, LevelableSkill::basic, 2.6380f, 0.2400f)},
					Node::Daze{.name = "Shooting Star Step 1st-Hit Daze", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.impact, LevelableSkill::basic, 0.8270f, 0.0380f)},
					Node::Daze{.name = "Shooting Star Step 2nd-Hit Daze", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.impact, LevelableSkill::basic, 0.8410f, 0.0390f)},
					Node::Daze{.name = "Shooting Star Step 3rd-Hit Daze", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.impact, LevelableSkill::basic, 1.6890f, 0.0770f)},
					Node::Atk{.name = "Adorable Explosive Impact 1st-Hit DMG", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.atk, LevelableSkill::basic, 1.0400f, 0.0950f)},
					Node::Atk{.name = "Adorable Explosive Impact 2nd-Hit DMG", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.atk, LevelableSkill::basic, 3.0440f, 0.2770f)},
					Node::Atk{.name = "Adorable Explosive Impact 3rd-Hit DMG", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.atk, LevelableSkill::basic, 5.1080f, 0.4650f)},
					Node::Atk{.name = "Adorable Explosive Impact 3rd-Hit DMG (Coordinated)", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.atk, LevelableSkill::basic, 5.1080f, 0.4650f) + Multiplier(combat.atk, LevelableSkill::basic, 0.7670f, 0.07f)},
					Node::Daze{.name = "Adorable Explosive Impact 1st-Hit Daze", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.impact, LevelableSkill::basic, 0.4400f, 0.0200f)},
					Node::Daze{.name = "Adorable Explosive Impact 2nd-Hit Daze", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.impact, LevelableSkill::basic, 2.1080f, 0.0960f)},
					Node::Daze{.name = "Adorable Explosive Impact 3rd-Hit Daze", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.impact, LevelableSkill::basic, 4.1080f, 0.1870f)},
					Node::Daze{.name = "Adorable Explosive Impact 3rd-Hit Daze (Coordinated)", .source = Misc::AttackSource::basic, .formula = Multiplier(combat.impact, LevelableSkill::basic, 4.1080f, 0.1870f) + Multiplier(combat.impact, LevelableSkill::basic, 0.2060f, 0.01f)},
				},
				.dashAttack{
					Node::Atk{.name = "DMG", .source = Misc::AttackSource::dodge, .formula = Multiplier(combat.atk, LevelableSkill::dodge, 1.9070f, 0.1740f)},
					Node::Daze{.name = "Daze", .source = Misc::AttackSource::dodge, .formula = Multiplier(combat.impact, LevelableSkill::dodge, 0.2570f, 0.0120f)},
				},
				.dodgeCounter{
					Node::Atk{.name = "DMG", .source = Misc::AttackSource::dodge, .formula = Multiplier(combat.atk, LevelableSkill::dodge, 3.5530f, 0.3230f)},
					Node::Daze{.name = "Daze", .source = Misc::AttackSource::dodge, .formula = Multiplier(combat.impact, LevelableSkill::dodge, 2.5670f, 0.1170f)},
				},
				.quickAssist{
					Node::Atk{.name = "DMG", .source = Misc::AttackSource::assist, .formula = Multiplier(combat.atk, LevelableSkill::assist, 0.7480f, 0.0680f)},
					Node::Daze{.name = "Daze", .source = Misc::AttackSource::assist, .formula = Multiplier(combat.impact, LevelableSkill::assist, 0.6330f, 0.0290f)},
				},
				.defensiveAssist{
					Node::Daze{.name = "Light Defensive Daze", .source = Misc::AttackSource::assist, .formula = Multiplier(combat.impact, LevelableSkill::assist, 2.7130f, 0.1240f)},
					Node::Daze{.name = "Heavy Defensive Daze", .source = Misc::AttackSource::assist, .formula = Multiplier(combat.impact, LevelableSkill::assist, 3.4280f, 0.1560f)},
					Node::Daze{.name = "Chain Defensive Daze", .source = Misc::AttackSource::assist, .formula = Multiplier(combat.impact, LevelableSkill::assist, 1.6680f, 0.0760f)},
				},
				.assistFollowup{
					Node::Atk{.name = "DMG", .source = Misc::AttackSource::assist, .formula = Multiplier(combat.atk, LevelableSkill::assist, 5.3580f, 0.4880f)},
					Node::Daze{.name = "Daze", .source = Misc::AttackSource::assist, .formula = Multiplier(combat.impact, LevelableSkill::assist, 3.9920f, 0.1820f)},
				},
				.special{
					Node::Atk{.name = "DMG", .source = Misc::AttackSource::special, .formula = Multiplier(combat.atk, LevelableSkill::special, 0.8350f, 0.0760f)},
					Node::Daze{.name = "Daze", .source = Misc::AttackSource::special, .formula = Multiplier(combat.impact, LevelableSkill::special, 0.7070f, 0.0330f)},
				},
				.exspecial{
					Node::Atk{.name = "DMG", .source = Misc::AttackSource::special, .formula = Multiplier(combat.atk, LevelableSkill::special, 5.3300f, 0.4850f)},
					Node::Daze{.name = "Daze", .source = Misc::AttackSource::special, .formula = Multiplier(combat.impact, LevelableSkill::special, 4.1430f, 0.1890f)},
					Node::Info{.name = "Energy Cost", .type = Utils::EntryType::energy, .formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 40.f, 0.f)},
				},
				.chain{
					Node::Atk{.name = "DMG", .source = Misc::AttackSource::chain, .formula = Multiplier(combat.atk, LevelableSkill::chain, 7.9820f, 0.7260f)},
					Node::Daze{.name = "Daze", .source = Misc::AttackSource::chain, .formula = Multiplier(combat.impact, LevelableSkill::chain, 2.3650f, 0.1080f)},
				},
				.ultimate{
					Node::Atk{.name = "DMG", .source = Misc::AttackSource::chain, .formula = Multiplier(combat.atk, LevelableSkill::chain, 21.0170f, 1.9110f)},
					Node::Daze{.name = "Daze", .source = Misc::AttackSource::chain, .formula = Multiplier(combat.impact, LevelableSkill::chain, 11.9610f, 0.5440f)},
				},
				.core = [&]() {
					std::vector<Node::Types> ret{
						Node::Mods{
							.mods{
								.combat{
									.ap = multiplier1,
									.impact = coreImpactBuff,
								},
							},
						},
					};
					Formula::addAblooms(//
						std::array<Formula::FloatNode, 5>{
							(1.f + abloomAdditionalMult) * 0.63f,
							(1.f + abloomAdditionalMult) * 9.f,
							(1.f + abloomAdditionalMult) * 0.9f,
							(1.f + abloomAdditionalMult) * 3.6f,
							(1.f + abloomAdditionalMult) * 7.2f,
						},
						ret
					);
					Formula::addPolarityDisorders(Constant{.value = 0.25f}, ret);
					return ret;
				}(),
				.mindscape4{
					Node::Mods{
						.mods{
							.combat{
								.ap = m4Buff,
							},
						},
					},
				},
				.mindscape6 = [&]() {
					std::vector<Node::Types> ret{
						Node::Mods{
							.mods{
								.combat{
									.all{
										.daze = m6DazeBuff,
									},
								},
							},
						},
					};
					Formula::addAblooms(//
						std::array<Formula::FloatNode, 5>{
							(1.f + m6VibratoBuff) * 0.7f,
							(1.f + m6VibratoBuff) * 10.f,
							(1.f + m6VibratoBuff) * 1.f,
							(1.f + m6VibratoBuff) * 4.0f,
							(1.f + m6VibratoBuff) * 8.f,
						},
						ret
					);
					return ret;
				}(),
			},
		};
	},
};
