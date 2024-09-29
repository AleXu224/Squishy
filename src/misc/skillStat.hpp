#pragma once

#include "cstdint"
#include "utils/isPercentage.hpp"
#include "utils/stringify.hpp"

namespace Misc {
	enum class SkillStat : uint8_t {
		DMG,
		additiveDMG,
		multiplicativeDMG,
		critRate,
		critDMG,
	};
}

namespace Utils {
	template<>
	[[nodiscard]] constexpr bool isPercentage<Misc::SkillStat>(const Misc::SkillStat &stat) {
		switch (stat) {
			case Misc::SkillStat::additiveDMG:
				return false;
			default:
				return true;
		}
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
	}
}// namespace Utils