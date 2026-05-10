#include "Alice.hpp"

#include "agent/setup.hpp"
#include "ranges"

const Agent::Data Agent::Datas::alice{
	.key{1401},
	.goodKey{"Alice"},
	.name{"Alice"},
	.baseStats{
		.baseHp = 617,
		.baseAtk = 127,
		.baseDef = 49,
		.baseImpact = 86,
		.baseAm = 106,
		.baseAp = 118,
		.coreStat1 = Stat::atk,
		.coreStat2 = Stat::am,
		.rarity = 5,
		.attribute = Misc::Attribute::physical,
		.specialty = Misc::Specialty::anomaly,
		.hpGrowth = 83.7238,
		.atkGrowth = 7.6728,
		.defGrowth = 6.6203,
		.hpUpgrade = {0.000, 423.000, 847.000, 1270.000, 1694.000, 2117.000},
		.atkUpgrade = {0.000, 45.000, 90.000, 136.000, 181.000, 226.000},
		.defUpgrade = {0.000, 33.000, 67.000, 100.000, 134.000, 167.000},
		.coreStat1Upgrade = {0.000, 25.000, 25.000, 50.000, 50.000, 75.000},
		.coreStat2Upgrade = {12.000, 12.000, 24.000, 24.000, 36.000, 36.000},
	},
	.setup = []() -> Data::Setup {
		auto coreAnomalyTimer = GetFloat("aliceAnomalyTimer");
		auto coreDisorderBuff = coreAnomalyTimer * 0.18f * combat.atk;

		return Data::Setup{
			.mods{
				.combat{
					.assaultDisorder{
						.additiveDMG = coreDisorderBuff,
					},
				},
			},
			.opts{
				.core{
					Option::ValueSlider{
						.key = "aliceAnomalyTimer",
						.name = "Physical Anomaly time left",
						.values = std::views::iota(0, 11) | std::ranges::to<std::vector<float>>(),
						.mods{
							.combat{
								.assaultDisorder{
									.additiveDMG = coreDisorderBuff,
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
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.5510f, 0.0510f)
					},
					Node::Atk{
						.name = "2nd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.8800f, 0.0800f)
					},
					Node::Atk{
						.name = "3rd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.6610f, 0.0610f)
					},
					Node::Atk{
						.name = "4th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.1190f, 0.1020f)
					},
					Node::Atk{
						.name = "5th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.3030f, 0.1190f)
					},
					Node::Atk{
						.name = "5th-Hit (Enhanced) DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 2.1830f, 0.1990f)
					},
					Node::Daze{
						.name = "1st-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.3970f, 0.0190f)
					},
					Node::Daze{
						.name = "2nd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.6980f, 0.0320f)
					},
					Node::Daze{
						.name = "3rd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.5990f, 0.0280f)
					},
					Node::Daze{
						.name = "4th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.0510f, 0.0480f)
					},
					Node::Daze{
						.name = "5th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.3170f, 0.0600f)
					},
					Node::Daze{
						.name = "5th-Hit (Enhanced) Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 2.1090f, 0.0960f)
					},
					Node::Atk{
						.name = "One-Staged Charged Attack DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.8890f, 0.1720f)
					},
					Node::Atk{
						.name = "Two-Staged Charged Attack DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 3.0440f, 0.2770f)
					},
					Node::Atk{
						.name = "Three-Staged Charged Attack DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 9.6570f, 0.8780f)
					},
					Node::Daze{
						.name = "One-Staged Charged Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.5530f, 0.0260f)
					},
					Node::Daze{
						.name = "Two-Staged Charged Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.7760f, 0.0360f)
					},
					Node::Daze{
						.name = "Three-Staged Charged Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.9720f, 0.0900f)
					},
				},
				.dashAttack{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 0.7530f, 0.0690f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 0.3390f, 0.0160f)
					},
				},
				.dodgeCounter{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 2.8400f, 0.2590f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 2.1950f, 0.1000f)
					},
				},
				.quickAssist{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 1.6330f, 0.1490f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 1.4690f, 0.0670f)
					},
				},
				.defensiveAssist{
					Node::Daze{
						.name = "Light Defensive Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 2.4420f, 0.1110f)
					},
					Node::Daze{
						.name = "Heavy Defensive Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 3.0860f, 0.1410f)
					},
					Node::Daze{
						.name = "Chain Defensive Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 1.5020f, 0.0690f)
					},
				},
				.assistFollowup{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 3.3270f, 0.3030f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 2.5840f, 0.1180f)
					},
				},
				.special{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 0.6240f, 0.0570f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.5610f, 0.0260f)
					},
				},
				.exspecial{
					Node::Atk{
						.name = "Aurora Thrust - Northern Cross DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 4.6000f, 0.4190f)
					},
					Node::Daze{
						.name = "Aurora Thrust - Northern Cross Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 3.4520f, 0.1570f)
					},
					Node::Info{
						.name = "Aurora Thrust - Northern Cross Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 40.f, 0.f)
					},
					Node::Atk{
						.name = "Aurora Thrust - Southern Cross DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 5.3200f, 0.4840f)
					},
					Node::Daze{
						.name = "Aurora Thrust - Southern Cross Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 4.0530f, 0.1850f)
					},
					Node::Info{
						.name = "Aurora Thrust - Southern Cross Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 40.f, 0.f)
					},
				},
				.chain{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 6.6630f, 0.6060f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 2.0470f, 0.0940f)
					},
				},
				.ultimate{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 22.6200f, 2.0570f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 2.4260f, 0.1110f)
					},
				},
			},
		};
	},
};
