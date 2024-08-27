#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<FloatFormula T, FloatFormula U>
	struct Fraction {
		T val1;
		U val2;

		[[nodiscard]] inline std::string print(const Context &context, Step prevStep) const {
			auto r2 = val2.eval(context);

			if (r2 == 1.f) return fmt::format("{}", val1.print(context, prevStep));

			return fmt::format("{} / {}", val1.print(context, Step::division), val2.print(context, Step::division));
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			return val1.eval(context) / val2.eval(context);
		}
	};
}// namespace Formula