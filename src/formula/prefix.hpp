#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<FloatFormula T>
	struct Prefix {
		std::string_view prefix;
		T val;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			return fmt::format("{} {}", prefix, val.eval(stats, team));
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return val.eval(stats, team);
		}
	};
}// namespace Formula