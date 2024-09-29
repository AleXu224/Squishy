#pragma once

#include "cstdint"
#include "utils/isPercentage.hpp"
#include "utils/stringify.hpp"


namespace Misc {
	enum class EnemyStat : uint8_t {
		level,
		DEFReduction,
		DEFIgnored,
	};
	enum class EnemyResistances : uint8_t {
		resistance,
	};
}// namespace Misc

namespace Utils {
	template<>
	[[nodiscard]] inline std::string Stringify<Misc::EnemyStat>(const Misc::EnemyStat &stats) {
		switch (stats) {
			case Misc::EnemyStat::level:
				return "Level";
			case Misc::EnemyStat::DEFReduction:
				return "DEF Reduction";
			case Misc::EnemyStat::DEFIgnored:
				return "DEF Ignored";
		}
	}
	template<>
	[[nodiscard]] inline std::string Stringify<Misc::EnemyResistances>(const Misc::EnemyResistances &resistance) {
		switch (resistance) {
			case Misc::EnemyResistances::resistance:
				return "RES";
		}
	}
	template<>
	[[nodiscard]] constexpr bool isPercentage<Misc::EnemyStat>(const Misc::EnemyStat &stat) {
		switch (stat) {
			case Misc::EnemyStat::level:
				return false;
			case Misc::EnemyStat::DEFReduction:
			case Misc::EnemyStat::DEFIgnored:
				return true;
		}
	}
}// namespace Utils