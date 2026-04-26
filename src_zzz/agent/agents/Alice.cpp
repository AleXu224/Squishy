#include "Alice.hpp"

#include "agent/setup.hpp"

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
        return Data::Setup{
            .mods{},
            .opts{},
            .nodes{
                .basic{
                    Node::Atk{
                        .name = "1st-Hit DMG Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 0.5510f, 0.0510f)
                    },
                    Node::Atk{
                        .name = "2nd-Hit DMG Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 0.8800f, 0.0800f)
                    },
                    Node::Atk{
                        .name = "3rd-Hit DMG Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 0.6610f, 0.0610f)
                    },
                    Node::Atk{
                        .name = "4th-Hit DMG Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 1.1190f, 0.1020f)
                    },
                    Node::Atk{
                        .name = "5th-Hit DMG Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 1.3030f, 0.1190f)
                    },
                    Node::Atk{
                        .name = "5th-Hit (Enhanced) DMG Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 2.1830f, 0.1990f)
                    },
                    Node::Atk{
                        .name = "1st-Hit Daze Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 0.3970f, 0.0190f)
                    },
                    Node::Atk{
                        .name = "2nd-Hit Daze Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 0.6980f, 0.0320f)
                    },
                    Node::Atk{
                        .name = "3rd-Hit Daze Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 0.5990f, 0.0280f)
                    },
                    Node::Atk{
                        .name = "4th-Hit Daze Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 1.0510f, 0.0480f)
                    },
                    Node::Atk{
                        .name = "5th-Hit Daze Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 1.3170f, 0.0600f)
                    },
                    Node::Atk{
                        .name = "5th-Hit (Enhanced) Daze Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 2.1090f, 0.0960f)
                    },
                    Node::Atk{
                        .name = "One-Staged Charged Attack DMG Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 1.8890f, 0.1720f)
                    },
                    Node::Atk{
                        .name = "Two-Staged Charged Attack DMG Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 3.0440f, 0.2770f)
                    },
                    Node::Atk{
                        .name = "Three-Staged Charged Attack DMG Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 9.6570f, 0.8780f)
                    },
                    Node::Atk{
                        .name = "One-Staged Charged Daze Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 0.5530f, 0.0260f)
                    },
                    Node::Atk{
                        .name = "Two-Staged Charged Daze Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 0.7760f, 0.0360f)
                    },
                    Node::Atk{
                        .name = "Three-Staged Charged Daze Multiplier",
						.source = Misc::AttackSource::basic,
                        .formula = Multiplier(combat.atk, LevelableSkill::basic, 1.9720f, 0.0900f)
                    },
                },
                .dodge{
                    Node::Atk{
                        .name = "DMG Multiplier",
						.source = Misc::AttackSource::dodge,
                        .formula = Multiplier(combat.atk, LevelableSkill::dodge, 0.7530f, 0.0690f)
                    },
                    Node::Atk{
                        .name = "Daze Multiplier",
						.source = Misc::AttackSource::dodge,
                        .formula = Multiplier(combat.atk, LevelableSkill::dodge, 0.3390f, 0.0160f)
                    },
                    Node::Atk{
                        .name = "DMG Multiplier",
						.source = Misc::AttackSource::dodge,
                        .formula = Multiplier(combat.atk, LevelableSkill::dodge, 2.8400f, 0.2590f)
                    },
                    Node::Atk{
                        .name = "Daze Multiplier",
						.source = Misc::AttackSource::dodge,
                        .formula = Multiplier(combat.atk, LevelableSkill::dodge, 2.1950f, 0.1000f)
                    },
                },
                .assist{
                    Node::Atk{
                        .name = "DMG Multiplier",
						.source = Misc::AttackSource::assist,
                        .formula = Multiplier(combat.atk, LevelableSkill::assist, 1.6330f, 0.1490f)
                    },
                    Node::Atk{
                        .name = "Daze Multiplier",
						.source = Misc::AttackSource::assist,
                        .formula = Multiplier(combat.atk, LevelableSkill::assist, 1.4690f, 0.0670f)
                    },
                    Node::Atk{
                        .name = "Light Defensive Daze Multiplier",
						.source = Misc::AttackSource::assist,
                        .formula = Multiplier(combat.atk, LevelableSkill::assist, 2.4420f, 0.1110f)
                    },
                    Node::Atk{
                        .name = "Heavy Defensive Daze Multiplier",
						.source = Misc::AttackSource::assist,
                        .formula = Multiplier(combat.atk, LevelableSkill::assist, 3.0860f, 0.1410f)
                    },
                    Node::Atk{
                        .name = "Chain Defensive Daze Multiplier",
						.source = Misc::AttackSource::assist,
                        .formula = Multiplier(combat.atk, LevelableSkill::assist, 1.5020f, 0.0690f)
                    },
                    Node::Atk{
                        .name = "DMG Multiplier",
						.source = Misc::AttackSource::assist,
                        .formula = Multiplier(combat.atk, LevelableSkill::assist, 3.3270f, 0.3030f)
                    },
                    Node::Atk{
                        .name = "Daze Multiplier",
						.source = Misc::AttackSource::assist,
                        .formula = Multiplier(combat.atk, LevelableSkill::assist, 2.5840f, 0.1180f)
                    },
                },
                .special{
                    Node::Atk{
                        .name = "DMG Multiplier",
						.source = Misc::AttackSource::special,
                        .formula = Multiplier(combat.atk, LevelableSkill::special, 0.6240f, 0.0570f)
                    },
                    Node::Atk{
                        .name = "Daze Multiplier",
						.source = Misc::AttackSource::special,
                        .formula = Multiplier(combat.atk, LevelableSkill::special, 0.5610f, 0.0260f)
                    },
                    Node::Atk{
                        .name = "DMG Multiplier",
						.source = Misc::AttackSource::special,
                        .formula = Multiplier(combat.atk, LevelableSkill::special, 4.6000f, 0.4190f)
                    },
                    Node::Atk{
                        .name = "Daze Multiplier",
						.source = Misc::AttackSource::special,
                        .formula = Multiplier(combat.atk, LevelableSkill::special, 3.4520f, 0.1570f)
                    },
                    Node::Info{
                        .name = "Energy Cost",
                        .type = Utils::EntryType::energy,
                        .formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 40.f, 0.f)
                    },
                    Node::Atk{
                        .name = "DMG Multiplier",
						.source = Misc::AttackSource::special,
                        .formula = Multiplier(combat.atk, LevelableSkill::special, 5.3200f, 0.4840f)
                    },
                    Node::Atk{
                        .name = "Daze Multiplier",
						.source = Misc::AttackSource::special,
                        .formula = Multiplier(combat.atk, LevelableSkill::special, 4.0530f, 0.1850f)
                    },
                    Node::Info{
                        .name = "Energy Cost",
                        .type = Utils::EntryType::energy,
                        .formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 40.f, 0.f)
                    },
                },
                .chain{
                    Node::Atk{
                        .name = "DMG Multiplier",
						.source = Misc::AttackSource::chain,
                        .formula = Multiplier(combat.atk, LevelableSkill::chain, 6.6630f, 0.6060f)
                    },
                    Node::Atk{
                        .name = "Daze Multiplier",
						.source = Misc::AttackSource::chain,
                        .formula = Multiplier(combat.atk, LevelableSkill::chain, 2.0470f, 0.0940f)
                    },
                    Node::Atk{
                        .name = "DMG Multiplier",
						.source = Misc::AttackSource::chain,
                        .formula = Multiplier(combat.atk, LevelableSkill::chain, 22.6200f, 2.0570f)
                    },
                    Node::Atk{
                        .name = "Daze Multiplier",
						.source = Misc::AttackSource::chain,
                        .formula = Multiplier(combat.atk, LevelableSkill::chain, 2.4260f, 0.1110f)
                    },
                },
            },
        };
    },
};
