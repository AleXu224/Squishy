#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<ArithmeticFormula T, ArithmeticFormula U>
	struct Product {
		T val1;
		U val2;

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			if (val1.eval(context) == 1.f) return val2.print(context, prevStep);
			if (val2.eval(context) == 1.f) return val1.print(context, prevStep);

			auto p1 = val1.print(context, Step::multiplication);
			auto p2 = val2.print(context, Step::multiplication);

			if (prevStep == Step::division) {
				return fmt::format("({} * {})", p1, p2);
			}

			return fmt::format("{} * {}", p1, p2);
		}

		[[nodiscard]] auto eval(const Context &context) const {
			return val1.eval(context) * val2.eval(context);
		}
	};
}// namespace Formula