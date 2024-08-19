#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<class T, class U>
	struct Product {
		T val1;
		U val2;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, Step prevStep) const {
			auto r1 = val1.eval(stats);
			auto r2 = val2.eval(stats);

			if (r1 == 1.f) return fmt::format("{}", val2.print(stats, prevStep));
			if (r2 == 1.f) return fmt::format("{}", val1.print(stats, prevStep));

			return fmt::format("{} * {}", val1.print(stats, Step::multiplication), val2.print(stats, Step::multiplication));
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats) const {
			return val1.eval(stats) * val2.eval(stats);
		}
	};
}// namespace Formula