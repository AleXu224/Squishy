#pragma once

#include "base.hpp"
#include "monomial.hpp"

namespace Formula {
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Sum : FormulaBase<SumType<typename T::RetType, typename V::RetType>> {
		T val1;
		V val2;

		using RetType = SumType<typename T::RetType, typename V::RetType>;

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto fold1 = val1.fold(ctx, args);
			auto fold2 = val2.fold(ctx, args);

			auto type1 = fold1.getType();
			auto type2 = fold2.getType();

			if (type1 == Type::constant && type2 == Type::constant) {
				return ConstantBase{.value = fold1.getConstantValue() + fold2.getConstantValue()};
			} else if (type1 == Type::constant && type2 == Type::none) {
				auto constValue = fold1.getConstantValue();
				if (constValue == 0) return fold2;
				return SumMonomial{.value = fold2}.add(constValue);
			} else if (type2 == Type::constant && type1 == Type::none) {
				auto constValue = fold2.getConstantValue();
				if (constValue == 0) return fold1;
				return SumMonomial{.value = fold1}.add(constValue);
			} else if (type1 == Type::constant && (type2 == Type::monomial || type2 == Type::summonomial || type2 == Type::prodmonomial)) {
				return fold2.add(fold1.getConstantValue());
			} else if (type2 == Type::constant && (type1 == Type::monomial || type1 == Type::summonomial || type1 == Type::prodmonomial)) {
				return fold1.add(fold2.getConstantValue());
			} else if (type1 == Type::summonomial && type2 == Type::summonomial) {
				auto mono1 = fold1.getMonomialValue();
				auto mono2 = fold2.getMonomialValue();

				return SumMonomial{.value = Sum<decltype(mono1), decltype(mono2)>{.val1 = mono1, .val2 = mono2}.fold(ctx, args)}
					.add(fold1.getSumParam() + fold2.getSumParam());
			}

			return Sum<decltype(fold1), decltype(fold2)>{
				.val1 = fold1,
				.val2 = fold2,
			};
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

		[[nodiscard]] RetType eval(const Formula::Context &context) const {
			return val1.eval(context) + val2.eval(context);
		}
	};
}// namespace Formula