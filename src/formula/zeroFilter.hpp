#pragma once

#include "intermediary.hpp"

namespace Formula {
	template<class T>
	struct ZeroFilter {
		T val;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, Step prevStep) const {
			if (val.eval(stats) == 0.f) return "";
			return val.print(stats, prevStep);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats) const {
			return val.eval(stats);
		}
	};
}// namespace Formula