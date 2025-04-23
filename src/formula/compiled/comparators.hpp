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
	};

	template<FormulaLike T, FormulaLike V>
	struct NotEqual : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) != val2.eval(context);
		}
	};

	template<BoolFormula T>
	struct Not : FormulaBase<bool> {
		T val;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return !val.eval(context);
		}
	};

	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Less : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) < val2.eval(context);
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct LessEqual : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) <= val2.eval(context);
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Greater : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) > val2.eval(context);
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct GreaterEqual : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) >= val2.eval(context);
		}
	};

	template<BoolFormula T, BoolFormula V>
	struct And : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) && val2.eval(context);
		}
	};
	template<BoolFormula T, BoolFormula V>
	struct Or : FormulaBase<bool> {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) || val2.eval(context);
		}
	};
}// namespace Formula::Compiled