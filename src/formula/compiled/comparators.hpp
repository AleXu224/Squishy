#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"

namespace Formula::Compiled {
	template<FormulaLike T, FormulaLike V>
	struct Equal {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) == val2.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return val1.isConstant() && val2.isConstant();
		}
	};

	template<FormulaLike T, FormulaLike V>
	struct NotEqual {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) != val2.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return val1.isConstant() && val2.isConstant();
		}
	};

	template<BoolFormula T>
	struct Not {
		T val;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return !val.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return val.isConstant();
		}
	};

	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Less {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) < val2.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return val1.isConstant() && val2.isConstant();
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct LessEqual {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) <= val2.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return val1.isConstant() && val2.isConstant();
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Greater {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) > val2.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return val1.isConstant() && val2.isConstant();
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct GreaterEqual {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) >= val2.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return val1.isConstant() && val2.isConstant();
		}
	};

	template<BoolFormula T, BoolFormula V>
	struct And {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) && val2.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return val1.isConstant() && val2.isConstant();
		}
	};
	template<BoolFormula T, BoolFormula V>
	struct Or {
		T val1;
		V val2;

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) || val2.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return val1.isConstant() && val2.isConstant();
		}
	};
}// namespace Formula::Compiled