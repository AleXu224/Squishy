#pragma once

#include "compiled/operators.hpp"// IWYU pragma: keep
#include "fmt/format.h"
#include "intermediary.hpp"

namespace Formula {
	inline std::string stepPrint(Step step) {
		switch (step) {
			case Step::addition:
				return "add";
			case Step::division:
				return "div";
			case Step::multiplication:
				return "mul";
			case Step::none:
				return "non";
		}
	}
	template<ArithmeticFormula T, ArithmeticFormula U>
	struct Sum {
		T val1;
		U val2;

		[[nodiscard]] inline auto compile(const Context &context) const {
			using namespace Compiled::Operators;
			return val1.compile(context) + val2.compile(context);
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			if (val1.eval(context) == 0.f) return val2.print(context, prevStep);
			if (val2.eval(context) == 0.f) return val1.print(context, prevStep);

			auto p1 = val1.print(context, Step::addition);
			auto p2 = val2.print(context, Step::addition);

			if (prevStep == Step::multiplication || prevStep == Step::division) {
				return fmt::format("({} + {})", p1, p2);
			}
			return fmt::format("{} + {}", p1, p2);
		}

		[[nodiscard]] auto eval(const Context &context) const {
			return val1.eval(context) + val2.eval(context);
		}
	};
}// namespace Formula