#pragma once

#include "cstdint"
#include "utility"
#include "utils/isPercentage.hpp"
#include "utils/stringify.hpp"
#include <vector>


namespace Misc {
	enum class SkillStat : uint8_t {
		DMG,
		directDMG,
		additiveDMG,
		additiveMultiplier,
		multiplicativeDMG,
		critRate,
		critDMG,
		daze,
	};

	static inline std::vector<SkillStat> skillStats{
		SkillStat::DMG,
		SkillStat::directDMG,
		SkillStat::additiveDMG,
		SkillStat::additiveMultiplier,
		SkillStat::multiplicativeDMG,
		SkillStat::critRate,
		SkillStat::critDMG,
		SkillStat::daze,
	};
}// namespace Misc

namespace Utils {
	template<>
	[[nodiscard]] constexpr bool isPercentage<Misc::SkillStat>(const Misc::SkillStat &stat) {
		switch (stat) {
			case Misc::SkillStat::additiveDMG:
				return false;
			default:
				return true;
		}
		std::unreachable();
	}
	template<>
	[[nodiscard]] inline std::string Stringify<Misc::SkillStat>(const Misc::SkillStat &stat) {
		switch (stat) {
			case Misc::SkillStat::DMG:
				return "DMG";
			case Misc::SkillStat::directDMG:
				return "Direct DMG";
			case Misc::SkillStat::additiveDMG:
				return "Additional DMG";
			case Misc::SkillStat::additiveMultiplier:
				return "Additional Multiplier";
			case Misc::SkillStat::multiplicativeDMG:
				return "Multiplicative DMG";
			case Misc::SkillStat::critRate:
				return "Crit Rate";
			case Misc::SkillStat::critDMG:
				return "Crit DMG";
			case Misc::SkillStat::daze:
				return "Daze";
		}
		std::unreachable();
	}
}// namespace Utils