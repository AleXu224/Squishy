#pragma once

#include "base.hpp"
#include "monomial.hpp"

namespace Formula {
	template<ArithmeticFormula T, ArithmeticFormula U>
	struct Fraction : FormulaBase<FractionType<typename T::RetType, typename U::RetType>> {
		T val1;
		U val2;

		using RetType = FractionType<typename T::RetType, typename U::RetType>;
		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto fold1 = val1.fold(ctx, args);
			auto fold2 = val2.fold(ctx, args);

			auto type1 = fold1.getType();
			auto type2 = fold2.getType();

			if (type1 == Type::constant && type2 == Type::constant) {
				return ConstantBase{.value = fold1.getConstantValue() / fold2.getConstantValue()};
			} else if (type1 == Type::constant && type2 == Type::none) {
				auto constValue = fold1.getConstantValue();
				// Return the zero
				if (constValue == 0) return fold1;
				// return ProdMonomial{.value = fold2}.divide(fold1.getConstantValue());
			} else if (type2 == Type::constant && type1 == Type::none) {
				return ProdMonomial{.value = fold1}.divide(fold2.getConstantValue());
			} else if (type1 == Type::constant && (type2 == Type::monomial || type2 == Type::summonomial || type2 == Type::prodmonomial)) {
				auto constValue = fold1.getConstantValue();
				// Return the zero
				if (constValue == 0) return fold1;
				// return fold2.divide(fold1.getConstantValue());
			} else if (type2 == Type::constant && (type1 == Type::monomial || type1 == Type::summonomial || type1 == Type::prodmonomial)) {
				return fold1.divide(fold2.getConstantValue());
			} else if (type1 == Type::prodmonomial && type2 == Type::prodmonomial) {
				auto mono1 = fold1.getMonomialValue();
				auto mono2 = fold2.getMonomialValue();

				return ProdMonomial{.value = Fraction<decltype(mono1), decltype(mono2)>{.val1 = fold1.getMonomialValue(), .val2 = fold2.getMonomialValue()}}
					.mult(fold1.getMultParam() / fold2.getMultParam());
			}

			return Fraction<decltype(fold1), decltype(fold2)>{
				.val1 = fold1,
				.val2 = fold2,
			};
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