#pragma once
#include "curves.hpp"
#include "stat.hpp"
#include "statSheet.hpp"
#include "string"
#include <array>
#include <functional>
#include <unordered_map>
#include "node.hpp"

namespace Squishy {
    enum class WeaponType {
        Sword,
        Claymore,
        Polearm,
        Bow,
        Catalyst,
    };
    
    struct IWeaponData {
        const std::string name;
        struct BaseStats {
            float baseATK{0};
            float subStatValue{0};
            ArtifactStat subStat;
            WeaponType type;
            WeaponGrowCurve atkCurve;
            WeaponGrowCurve subStatCurve;
            std::array<float, 7> atkUpgrade;
        };
        const BaseStats baseStats;

        struct Conditional {
            std::string name;
            bool value = false;

            operator bool() const {
                return value;
            }
        };
		using Conditionals = std::unordered_map<std::string, Conditional>;
        std::function<Conditionals(StatSheet &)> conditionalsSetup;
		std::function<void(StatSheet &, const Conditionals &)> modsSetup;
		std::function<std::vector<Node>(StatSheet &,const Conditionals &)> nodeSetup;

        void addStatSheetData(StatSheet &sheet) const {
			sheet.base.ATK += baseStats.baseATK * WeaponCurves.at(static_cast<size_t>(baseStats.atkCurve)).at(sheet.weaponLevel - 1) + baseStats.atkUpgrade.at(sheet.weaponAscension);
            sheet.stats.getStat(baseStats.subStat) += baseStats.subStatValue * WeaponCurves.at(static_cast<size_t>(baseStats.subStatCurve)).at(sheet.weaponLevel - 1);
		}
	};
}