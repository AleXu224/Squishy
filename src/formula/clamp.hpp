#pragma once

#include "algorithm"
#include "fmt/core.h"
#include "formula/intermediary.hpp"


namespace Formula {
	template<FloatFormula T>
	struct Clamp {
		T val1;
		float min = 0.f;
		float max = 1.f;
		bool isPercentage = true;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			auto val = val1.eval(context);
			if (val >= min && val <= max) return val1.print(context, Step::none);
			return fmt::format("clamp({}, {}, {})", val1.print(context, Step::none), min, max);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			return std::clamp(val1.eval(context), min, max);
		}
	};
}// namespace Formula