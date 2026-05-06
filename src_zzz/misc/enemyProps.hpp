#pragma once

#include "cstdint"
#include "utility"
#include "utils/isPercentage.hpp"
#include "utils/stringify.hpp"


namespace Misc {
	enum class EnemyStat : uint8_t {
		level,
		baseDef,
		DEFReduction,
		DEFIgnored,
		dazeRes,
		daze,
		dazeTaken,
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
			case Misc::EnemyStat::baseDef:
				return "Base DEF";
			case Misc::EnemyStat::DEFReduction:
				return "DEF Reduction";
			case Misc::EnemyStat::DEFIgnored:
				return "DEF Ignored";
			case Misc::EnemyStat::dazeRes:
				return "Daze RES";
			case Misc::EnemyStat::daze:
				return "Daze%";
			case Misc::EnemyStat::dazeTaken:
				return "Daze Taken%";
		}
		std::unreachable();
	}
	template<>
	[[nodiscard]] inline std::string Stringify<Misc::EnemyResistances>(const Misc::EnemyResistances &resistance) {
		switch (resistance) {
			case Misc::EnemyResistances::resistance:
				return "RES";
		}
		std::unreachable();
	}
	template<>
	[[nodiscard]] constexpr bool isPercentage<Misc::EnemyStat>(const Misc::EnemyStat &stat) {
		switch (stat) {
			case Misc::EnemyStat::level:
			case Misc::EnemyStat::baseDef:
				return false;
			case Misc::EnemyStat::DEFReduction:
			case Misc::EnemyStat::DEFIgnored:
			case Misc::EnemyStat::dazeRes:
			case Misc::EnemyStat::daze:
			case Misc::EnemyStat::dazeTaken:
				return true;
		}
		std::unreachable();
	}
}// namespace Utils