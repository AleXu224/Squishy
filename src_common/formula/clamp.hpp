#pragma once

#include "base.hpp"

namespace Formula {
	template<ArithmeticFormula T>
	struct Clamp : FormulaBase<FormulaType<T>> {
		using RetType = FormulaType<T>;
		T val1;
		RetType min = 0.f;
		RetType max = 1.f;
		bool isPercentage = true;

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto foldVal = val1.fold(ctx, args);
			auto typeVal = foldVal.getType();

			if (typeVal == Type::constant) {
				auto val = foldVal.getConstantValue();
				if (val < min) {
					return ConstantBase<RetType>{.value = min};
				} else if (val > max) {
					return ConstantBase<RetType>{.value = max};
				} else {
					return foldVal;
				}
			}

			return Clamp<decltype(foldVal)>{
				.val1 = foldVal,
				.min = min,
				.max = max,
				.isPercentage = isPercentage,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			auto val = val1.eval(context);
			if (val >= min && val <= max) return val1.print(context, prevStep);
			return fmt::format("clamp({}, {}, {})", val1.print(context, Step::none), min, max);
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			return std::clamp(val1.eval(context), min, max);
		}
	};

	template<ArithmeticFormula T, ArithmeticFormula U>
	struct Min : FormulaBase<FormulaType<T>> {
		using RetType = FormulaType<T>;
		T val1;
		U val2;
		bool isPercentage = true;

		static_assert(std::is_same_v<RetType, FormulaType<U>>, "Both formulas need to return the same type");

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto foldVal1 = val1.fold(ctx, args);
			auto foldVal2 = val2.fold(ctx, args);
			auto typeVal1 = foldVal1.getType();
			auto typeVal2 = foldVal2.getType();

			if (typeVal1 == Type::constant && typeVal2 == Type::constant) {
				auto val1 = foldVal1.getConstantValue();
				auto val2 = foldVal2.getConstantValue();
				if (val1 < val2) {
					return foldVal1;
				} else {
					return ConstantBase<RetType>{.value = val2};
				}
			}

			return Min<decltype(foldVal1), decltype(foldVal2)>{
				.val1 = foldVal1,
				.val2 = foldVal2,
				.isPercentage = isPercentage,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			auto eval1 = val1.eval(context);
			auto eval2 = val2.eval(context);
			if (eval1 < eval2) return val1.print(context, prevStep);
			return fmt::format("min({}, {})", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			return std::min(val1.eval(context), val2.eval(context));
		}
	};

	template<ArithmeticFormula T, ArithmeticFormula U>
	struct Max : FormulaBase<FormulaType<T>> {
		using RetType = FormulaType<T>;
		T val1;
		U val2;
		bool isPercentage = true;

		static_assert(std::is_same_v<RetType, FormulaType<U>>, "Both formulas need to return the same type");

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto foldVal1 = val1.fold(ctx, args);
			auto foldVal2 = val2.fold(ctx, args);
			auto typeVal1 = foldVal1.getType();
			auto typeVal2 = foldVal2.getType();

			if (typeVal1 == Type::constant && typeVal2 == Type::constant) {
				auto val1 = foldVal1.getConstantValue();
				auto val2 = foldVal2.getConstantValue();
				if (val1 > val2) {
					return foldVal1;
				} else {
					return ConstantBase<RetType>{.value = val2};
				}
			}

			return Max<decltype(foldVal1), decltype(foldVal2)>{
				.val1 = foldVal1,
				.val2 = foldVal2,
				.isPercentage = isPercentage,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			auto eval1 = val1.eval(context);
			auto eval2 = val2.eval(context);
			if (eval1 > eval2) return val1.print(context, prevStep);
			return fmt::format("max({}, {})", val1.print(context, Step::none), val2.print(context, Step::none));
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			return std::max(val1.eval(context), val2.eval(context));
		}
	};
}// namespace Formula