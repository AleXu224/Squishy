#pragma once

#include "base.hpp"


namespace Formula {
	template<FormulaLike T, FormulaLike V>
	struct Equal : FormulaBase<bool> {
		T val1;
		V val2;

		BoolNode fold(const Context &ctx, const FoldArgs &args) const {
			auto fold1 = val1.fold(ctx, args);
			auto fold2 = val2.fold(ctx, args);

			auto type1 = fold1.getType();
			auto type2 = fold2.getType();

			if (type1 == Type::constant && type2 == Type::constant) {
				return ConstantBase{.value = fold1.getConstantValue() == fold2.getConstantValue()};
			}

			return Equal<decltype(fold1), decltype(fold2)>{.val1 = fold1, .val2 = fold2};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return std::format("{} == {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) == val2.eval(context);
		}
	};

	template<FormulaLike T, FormulaLike V>
	struct NotEqual : FormulaBase<bool> {
		T val1;
		V val2;

		BoolNode fold(const Context &ctx, const FoldArgs &args) const {
			auto fold1 = val1.fold(ctx, args);
			auto fold2 = val2.fold(ctx, args);

			auto type1 = fold1.getType();
			auto type2 = fold2.getType();

			if (type1 == Type::constant && type2 == Type::constant) {
				return ConstantBase{.value = fold1.getConstantValue() != fold2.getConstantValue()};
			}

			return NotEqual<decltype(fold1), decltype(fold2)>{.val1 = fold1, .val2 = fold2};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return std::format("{} != {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) != val2.eval(context);
		}
	};

	template<BoolFormula T>
	struct Not : FormulaBase<bool> {
		T val;

		BoolNode fold(const Context &ctx, const FoldArgs &args) const {
			auto fold = val.fold(ctx, args);

			auto type1 = fold.getType();

			if (type1 == Type::constant) {
				return ConstantBase{.value = !fold.getConstantValue()};
			}

			return Not<decltype(fold)>{.val = fold};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return std::format("!{}", val.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return !val.eval(context);
		}
	};

	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Less : FormulaBase<bool> {
		T val1;
		V val2;

		BoolNode fold(const Context &ctx, const FoldArgs &args) const {
			auto fold1 = val1.fold(ctx, args);
			auto fold2 = val2.fold(ctx, args);

			auto type1 = fold1.getType();
			auto type2 = fold2.getType();

			if (type1 == Type::constant && type2 == Type::constant) {
				return ConstantBase{.value = fold1.getConstantValue() < fold2.getConstantValue()};
			}

			return Less<decltype(fold1), decltype(fold2)>{.val1 = fold1, .val2 = fold2};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return std::format("{} < {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) < val2.eval(context);
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct LessEqual : FormulaBase<bool> {
		T val1;
		V val2;

		BoolNode fold(const Context &ctx, const FoldArgs &args) const {
			auto fold1 = val1.fold(ctx, args);
			auto fold2 = val2.fold(ctx, args);

			auto type1 = fold1.getType();
			auto type2 = fold2.getType();

			if (type1 == Type::constant && type2 == Type::constant) {
				return ConstantBase{.value = fold1.getConstantValue() <= fold2.getConstantValue()};
			}

			return LessEqual<decltype(fold1), decltype(fold2)>{.val1 = fold1, .val2 = fold2};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return std::format("{} <= {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) <= val2.eval(context);
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Greater : FormulaBase<bool> {
		T val1;
		V val2;

		BoolNode fold(const Context &ctx, const FoldArgs &args) const {
			auto fold1 = val1.fold(ctx, args);
			auto fold2 = val2.fold(ctx, args);

			auto type1 = fold1.getType();
			auto type2 = fold2.getType();

			if (type1 == Type::constant && type2 == Type::constant) {
				return ConstantBase{.value = fold1.getConstantValue() > fold2.getConstantValue()};
			}

			return Greater<decltype(fold1), decltype(fold2)>{.val1 = fold1, .val2 = fold2};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return std::format("{} > {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) > val2.eval(context);
		}
	};
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct GreaterEqual : FormulaBase<bool> {
		T val1;
		V val2;

		BoolNode fold(const Context &ctx, const FoldArgs &args) const {
			auto fold1 = val1.fold(ctx, args);
			auto fold2 = val2.fold(ctx, args);

			auto type1 = fold1.getType();
			auto type2 = fold2.getType();

			if (type1 == Type::constant && type2 == Type::constant) {
				return ConstantBase{.value = fold1.getConstantValue() >= fold2.getConstantValue()};
			}

			return GreaterEqual<decltype(fold1), decltype(fold2)>{.val1 = fold1, .val2 = fold2};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return std::format("{} >= {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) >= val2.eval(context);
		}
	};

	template<BoolFormula T, BoolFormula V>
	struct And : FormulaBase<bool> {
		T val1;
		V val2;

		BoolNode fold(const Context &ctx, const FoldArgs &args) const {
			auto fold1 = val1.fold(ctx, args);
			auto fold2 = val2.fold(ctx, args);

			auto type1 = fold1.getType();
			auto type2 = fold2.getType();

			if (type1 == Type::constant && type2 == Type::constant) {
				return ConstantBool{.value = fold1.getConstantValue() && fold2.getConstantValue()};
			} else if (type1 == Type::constant && !fold1.getConstantValue()) {
				return ConstantBool{.value = false};
			} else if (type2 == Type::constant && !fold2.getConstantValue()) {
				return ConstantBool{.value = false};
			}

			return And<decltype(fold1), decltype(fold2)>{.val1 = fold1, .val2 = fold2};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return std::format("{} && {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) && val2.eval(context);
		}
	};
	template<BoolFormula T, BoolFormula V>
	struct Or : FormulaBase<bool> {
		T val1;
		V val2;

		BoolNode fold(const Context &ctx, const FoldArgs &args) const {
			auto fold1 = val1.fold(ctx, args);
			auto fold2 = val2.fold(ctx, args);

			auto type1 = fold1.getType();
			auto type2 = fold2.getType();

			if (type1 == Type::constant && type2 == Type::constant) {
				return ConstantBool{.value = fold1.getConstantValue() || fold2.getConstantValue()};
			} else if (type1 == Type::constant && fold1.getConstantValue()) {
				return ConstantBool{.value = true};
			} else if (type2 == Type::constant && fold2.getConstantValue()) {
				return ConstantBool{.value = true};
			}

			return Or<decltype(fold1), decltype(fold2)>{.val1 = fold1, .val2 = fold2};
		}

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return std::format("{} || {}", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] inline bool eval(const Context &context) const {
			return val1.eval(context) || val2.eval(context);
		}
	};
}// namespace Formula