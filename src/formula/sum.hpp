#pragma once

#include "fmt/format.h"
#include "intermediary.hpp"


namespace Formula {
	template<FloatFormula T, FloatFormula U>
	struct Sum {
		T val1;
		U val2;

		[[nodiscard]] inline std::string print(const Context &context, Step prevStep) const {
			auto p1 = val1.print(context, Step::addition);
			auto p2 = val2.print(context, Step::addition);

			if (val1.eval(context) == 0.f) return p2;
			if (val2.eval(context) == 0.f) return p1;

			if (prevStep == Step::multiplication || prevStep == Step::division) {
				return fmt::format("({} + {})", p1, p2);
			}
			return fmt::format("{} + {}", p1, p2);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			return val1.eval(context) + val2.eval(context);
		}
	};
}// namespace Formula