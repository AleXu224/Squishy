#pragma once

#include "intermediary.hpp"

namespace Formula {
	template<FloatFormula T>
	struct ZeroFilter {
		T val;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step prevStep) const {
			if (val.eval(stats, team) == 0.f) return "";
			return val.print(stats, team, prevStep);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return val.eval(stats, team);
		}
	};
}// namespace Formula