#pragma once

#include "intermediary.hpp"

namespace Formula {
	template<FloatFormula T>
	struct ZeroFilter {
		T val;

		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step prevStep) const {
			if (val.eval(source, target, team) == 0.f) return "";
			return val.print(source, target, team, prevStep);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return val.eval(source, target, team);
		}
	};
}// namespace Formula