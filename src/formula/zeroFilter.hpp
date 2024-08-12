#pragma once

#include "intermediary.hpp"

namespace Formula {
	template<class T>
	struct ZeroFilter {
		T val;

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step prevStep) const {
			if (val.eval(stats) == 0.f) return "";
			return val.print(stats, prevStep);
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			return val.eval(stats);
		}
	};
}// namespace Formula