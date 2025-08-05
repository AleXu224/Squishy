#pragma once

#include "cstdint"
#include "utility"
#include "utils/isPercentage.hpp"
#include "utils/stringify.hpp"
#include <vector>


namespace Misc {
	enum class SkillStat : uint8_t {
		DMG,
		additiveDMG,
		multiplicativeDMG,
		elevation,
		critRate,
		critDMG,
	};

	static inline std::vector<SkillStat> skillStats{
		SkillStat::DMG,
		SkillStat::additiveDMG,
		SkillStat::multiplicativeDMG,
		SkillStat::elevation,
		SkillStat::critRate,
		SkillStat::critDMG,
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
			case Misc::SkillStat::additiveDMG:
				return "Additional DMG";
			case Misc::SkillStat::multiplicativeDMG:
				return "Multiplicative DMG";
			case Misc::SkillStat::elevation:
				return "Elevation";
			case Misc::SkillStat::critRate:
				return "Crit Rate";
			case Misc::SkillStat::critDMG:
				return "Crit DMG";
		}
		std::unreachable();
	}
}// namespace Utils