#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<FloatFormula T, FloatFormula U>
	struct Product {
		T val1;
		U val2;

		[[nodiscard]] inline std::string print(const Context &context, Step prevStep) const {
			auto r1 = val1.eval(context);
			auto r2 = val2.eval(context);

			if (r1 == 1.f) return fmt::format("{}", val2.print(context, prevStep));
			if (r2 == 1.f) return fmt::format("{}", val1.print(context, prevStep));

			if (prevStep == Step::division) {
				return fmt::format("({} * {})", val1.print(context, Step::multiplication), val2.print(context, Step::multiplication));
			}

			return fmt::format("{} * {}", val1.print(context, Step::multiplication), val2.print(context, Step::multiplication));
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			return val1.eval(context) * val2.eval(context);
		}
	};
}// namespace Formula