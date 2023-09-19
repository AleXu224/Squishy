#pragma once
#include "stat.hpp"
#include "talent.hpp"
#include "element.hpp"
#include "curves.hpp"
#include "statSheet.hpp"
#include "node.hpp"
#include <vector>

namespace Squishy {
    struct ICharacterData {
        const std::string name;
        struct BaseStats {
            float baseHP{0};
            float baseATK{0};
            float baseDEF{0};
            ArtifactStat ascensionStat;
            LevelableTalent c3Talent;
            LevelableTalent c5Talent;
            Element element;
            CharacterGrowCurve hpCurve;
            CharacterGrowCurve atkCurve;
            CharacterGrowCurve defCurve;
            std::array<float, 7> hpUpgrade;
            std::array<float, 7> atkUpgrade;
            std::array<float, 7> defUpgrade;
            std::array<float, 7> ascensionStatUpgrade;
        };
        const BaseStats baseStats;

        struct TalentMultipliers {
            std::vector<std::vector<float>> normal;
            std::vector<std::vector<float>> skill;
            std::vector<std::vector<float>> burst;
            std::vector<std::vector<float>> passive1;
            std::vector<std::vector<float>> passive2;
            std::vector<std::vector<float>> constellation1;
            std::vector<std::vector<float>> constellation2;
            std::vector<std::vector<float>> constellation4;
            std::vector<std::vector<float>> constellation6;
        };
        const TalentMultipliers talentMultipliers;

        struct Conditional {
            std::string name;
            Talent location = Talent::Normal;
            // Optionally a list of values to use for the conditional
            std::vector<float> values{};
            float value = 0.f;
            bool active = false;

            operator bool() const {
                return active;
            }
        };
		using Conditionals = std::unordered_map<std::string, Conditional>;
        std::function<Conditionals(StatSheet &)> conditionalsSetup;
		std::function<void(StatSheet &, const TalentMultipliers &, const Conditionals&)> modsSetup;
        std::function<Nodes(StatSheet &, const TalentMultipliers &, const Conditionals&)> nodeSetup;

        void addStatSheetData(StatSheet &stats) const {
            stats.base.HP = baseStats.baseHP * CharacterCurves.at(static_cast<size_t>(baseStats.hpCurve)).at(stats.level - 1) + baseStats.hpUpgrade.at(stats.ascension);
            stats.base.ATK = baseStats.baseATK * CharacterCurves.at(static_cast<size_t>(baseStats.atkCurve)).at(stats.level - 1) + baseStats.atkUpgrade.at(stats.ascension);
            stats.base.DEF = baseStats.baseDEF * CharacterCurves.at(static_cast<size_t>(baseStats.defCurve)).at(stats.level - 1) + baseStats.defUpgrade.at(stats.ascension);
            stats.stats.getStat(baseStats.ascensionStat) += baseStats.ascensionStatUpgrade.at(stats.ascension);
			stats.element = baseStats.element;
            stats.stats.All.CritRate += 5.0_p;
            stats.stats.All.CritDMG += 50.0_p;
        }
    };
}