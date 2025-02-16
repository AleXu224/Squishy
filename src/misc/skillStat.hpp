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
		critRate,
		critDMG,
	};

	static inline std::vector<SkillStat> skillStats{
		SkillStat::DMG,
		SkillStat::additiveDMG,
		SkillStat::multiplicativeDMG,
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
				return "additiveDMG";
			case Misc::SkillStat::multiplicativeDMG:
				return "multiplicativeDMG";
			case Misc::SkillStat::critRate:
				return "critRate";
			case Misc::SkillStat::critDMG:
				return "critDMG";
		}
		std::unreachable();
	}
}// namespace Utils