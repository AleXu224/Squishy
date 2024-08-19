#pragma once

#include "cstdint"
#include "intermediary.hpp"
#include "stats/stat.hpp"


namespace Formula {
	struct ArtifactMainStat {
		::Stat stat;
		uint8_t level;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, Step prevStep) const {
			bool isPercentage = Stats::isPercentage(stat);
			return fmt::format("Artifact {} {}{}", Utils::Stringify(stat), eval(stats) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats) const {
			return Stats::Values::mainStat.at(stat).at(level);
		}
	};

	struct ArtifactSubStat {
		StatValue stat;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, Step prevStep) const {
			bool isPercentage = Stats::isPercentage(stat.stat);
			return fmt::format("Artifact {} {}{}", Utils::Stringify(stat), eval(stats) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats) const {
			return stat.value;
		}
	};
}// namespace Formula