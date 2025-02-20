#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<FormulaLike T, FormulaLike V>
	struct Equal {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("{} == {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) == val2.eval(context);
		}
	};

	template<FormulaLike T, FormulaLike V>
	struct NotEqual {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("{} != {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) != val2.eval(context);
		}
	};

	template<BoolFormula T>
	struct Not {
		T val;
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("!{}", val.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return !val.eval(context);
		}
	};

	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Less {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("{} < {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) < val2.eval(context);
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct LessEqual {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("{} <= {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) <= val2.eval(context);
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Greater {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("{} > {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) > val2.eval(context);
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct GreaterEqual {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("{} >= {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) >= val2.eval(context);
		}
	};

	template<BoolFormula T, BoolFormula V>
	struct And {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("{} && {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) && val2.eval(context);
		}
	};
	template<BoolFormula T, BoolFormula V>
	struct Or {
		T val1;
		V val2;
		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("{} || {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) || val2.eval(context);
		}
	};
}// namespace Formula