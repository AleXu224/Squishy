#pragma once

#include "cstdint"
#include "formula/formulaContext.hpp"
#include "stats/stat.hpp"
#include "step.hpp"


namespace Formula {
	struct ArtifactMainStat {
		::Stat stat;
		uint8_t level;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			bool isPercentage = Utils::isPercentage(stat);
			return fmt::format("Artifact {} {}{}", Utils::Stringify(stat), eval(context) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] float eval(const Context &) const {
			return Stats::Values::mainStat.at(stat).at(level);
		}
	};

	struct ArtifactSubStat {
		StatValue stat;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			bool isPercentage = Utils::isPercentage(stat.stat);
			return fmt::format("Artifact {} {}{}", Utils::Stringify(stat), eval(context) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] float eval(const Context &) const {
			return stat.value;
		}
	};
}// namespace Formula