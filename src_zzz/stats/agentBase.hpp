#pragma once

#include "array"
#include "misc/attribute.hpp"
#include "misc/specialty.hpp"
#include "stat.hpp"


namespace Stats {
	struct AgentBase {
		const float baseHp;
		const float baseAtk;
		const float baseDef;
		const float baseImpact;
		const float baseAm;
		const float baseAp;
		const Stat coreStat1;
		const Stat coreStat2;
		const uint8_t rarity;
		const Misc::Attribute attribute;
		const Misc::Specialty specialty;
		const float hpGrowth;
		const float atkGrowth;
		const float defGrowth;
		const std::array<float, 6> hpUpgrade;
		const std::array<float, 6> atkUpgrade;
		const std::array<float, 6> defUpgrade;
		const std::array<float, 6> coreStat1Upgrade;
		const std::array<float, 6> coreStat2Upgrade;

		[[nodiscard]] constexpr float getHpAt(unsigned short level, unsigned short promotion) const {
			return baseHp + (level - 1) * hpGrowth + hpUpgrade.at(promotion);
		}

		[[nodiscard]] constexpr float getAtkAt(unsigned short level, unsigned short promotion) const {
			return baseAtk + (level - 1) * atkGrowth + atkUpgrade.at(promotion);
		}

		[[nodiscard]] constexpr float getDefAt(unsigned short level, unsigned short promotion) const {
			return baseDef + (level - 1) * defGrowth + defUpgrade.at(promotion);
		}

		[[nodiscard]] constexpr float getCoreStat1At(unsigned short coreLvl) const {
			if (coreLvl == 0) return 0.f;
			return coreStat1Upgrade.at(coreLvl - 1);
		}

		[[nodiscard]] constexpr float getCoreStat2At(unsigned short coreLvl) const {
			if (coreLvl == 0) return 0.f;
			return coreStat2Upgrade.at(coreLvl - 1);
		}
	};

}// namespace Stats
