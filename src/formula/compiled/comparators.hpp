#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"

namespace Formula::Compiled {
	template<FormulaLike T, FormulaLike V>
	struct Equal : FormulaBase<bool, Type::none> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) == val2.eval(context);
		}

		[[nodiscard]] std::string print() const {
			return fmt::format("Equal<{}, {}>", val1.print(), val2.print());
		}
	};

	template<FormulaLike T, FormulaLike V>
	struct NotEqual : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) != val2.eval(context);
		}
		[[nodiscard]] std::string print() const {
			return fmt::format("NotEqual<{}, {}>", val1.print(), val2.print());
		}
	};

	template<BoolFormula T>
	struct Not : FormulaBase<bool> {
		T val;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return !val.eval(context);
		}
		[[nodiscard]] std::string print() const {
			return fmt::format("Not<{}>", val.print());
		}
	};

	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Less : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) < val2.eval(context);
		}

		[[nodiscard]] std::string print() const {
			return fmt::format("Less<{}, {}>", val1.print(), val2.print());
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct LessEqual : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) <= val2.eval(context);
		}

		[[nodiscard]] std::string print() const {
			return fmt::format("LessEqual<{}, {}>", val1.print(), val2.print());
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Greater : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) > val2.eval(context);
		}

		[[nodiscard]] std::string print() const {
			return fmt::format("Greater<{}, {}>", val1.print(), val2.print());
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct GreaterEqual : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) >= val2.eval(context);
		}

		[[nodiscard]] std::string print() const {
			return fmt::format("GreaterEqual<{}, {}>", val1.print(), val2.print());
		}
	};

	template<BoolFormula T, BoolFormula V>
	struct And : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) && val2.eval(context);
		}

		[[nodiscard]] std::string print() const {
			return fmt::format("And<{}, {}>", val1.print(), val2.print());
		}
	};
	template<BoolFormula T, BoolFormula V>
	struct Or : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) || val2.eval(context);
		}

		[[nodiscard]] std::string print() const {
			return fmt::format("Or<{}, {}>", val1.print(), val2.print());
		}
	};
}// namespace Formula::Compiled