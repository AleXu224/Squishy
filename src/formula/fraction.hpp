#pragma once

#include "compiled/operators.hpp"// IWYU pragma: keep
#include "fmt/core.h"
#include "intermediary.hpp"

namespace Formula {
	template<ArithmeticFormula T, ArithmeticFormula U>
	struct Fraction {
		T val1;
		U val2;

		[[nodiscard]] inline auto compile(const Context &context) const {
			using namespace Compiled::Operators;
			auto ret = val1.compile(context) / val2.compile(context);
			// if (ret.eval(context) != eval(context)) {
			// 	fmt::println("{} vs {}", ret.eval(context), eval(context));
			// }
			return ret;
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			auto r2 = val2.eval(context);

			if (r2 == 1.f) return fmt::format("{}", val1.print(context, prevStep));

			return fmt::format("{} / {}", val1.print(context, Step::division), val2.print(context, Step::division));
		}

		[[nodiscard]] auto eval(const Context &context) const {
			return val1.eval(context) / val2.eval(context);
		}
	};
}// namespace Formula