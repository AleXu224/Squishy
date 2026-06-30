#pragma once

#include "cstdint"
#include "misc/element.hpp"
#include "misc/skillStat.hpp"
#include "utility"
#include "utils/isPercentage.hpp"
#include "utils/overloaded.hpp"
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
	[[nodiscard]] inline std::string Stringify<std::pair<Misc::EnemyResistances, Misc::Element>>(const std::pair<Misc::EnemyResistances, Misc::Element> &resAttr) {
		return std::format("{} {}", Utils::Stringify(resAttr.second), Utils::Stringify(resAttr.first));
	}
	template<>
	[[nodiscard]] inline std::string Stringify<std::variant<Misc::SkillStat, Misc::EnemyStat, std::pair<Misc::EnemyResistances, Misc::Element>>>(const std::variant<Misc::SkillStat, Misc::EnemyStat, std::pair<Misc::EnemyResistances, Misc::Element>> &identifier) {
		return std::visit(
			Utils::overloaded{
				[](Misc::SkillStat skillStat) {
					return Utils::Stringify(skillStat);
				},
				[](Misc::EnemyStat enemyStat) {
					return Utils::Stringify(enemyStat);
				},
				[](std::pair<Misc::EnemyResistances, Misc::Element> resAttr) {
					return Utils::Stringify(resAttr);
				},
			},
			identifier
		);
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
		std::unreachable();
	}
	template<>
	[[nodiscard]] constexpr bool isPercentage<std::pair<Misc::EnemyResistances, Misc::Element>>(const std::pair<Misc::EnemyResistances, Misc::Element> &resAttr) {
		return true;
	}
	template<>
	[[nodiscard]] constexpr bool isPercentage<std::variant<Misc::SkillStat, Misc::EnemyStat, std::pair<Misc::EnemyResistances, Misc::Element>>>(const std::variant<Misc::SkillStat, Misc::EnemyStat, std::pair<Misc::EnemyResistances, Misc::Element>> &identifier) {
		return std::visit(
			Utils::overloaded{
				[](Misc::SkillStat skillStat) {
					return Utils::isPercentage(skillStat);
				},
				[](Misc::EnemyStat enemyStat) {
					return Utils::isPercentage(enemyStat);
				},
				[](std::pair<Misc::EnemyResistances, Misc::Element> resAttr) {
					return Utils::isPercentage(resAttr);
				},
			},
			identifier
		);
	}
}// namespace Utils