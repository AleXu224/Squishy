#pragma once

#include "base.hpp"
#include "monomial.hpp"

namespace Formula {
	template<ArithmeticFormula T, ArithmeticFormula U>
	struct Difference : FormulaBase<DifferenceType<typename T::RetType, typename U::RetType>> {
		T val1;
		U val2;

		using RetType = DifferenceType<typename T::RetType, typename U::RetType>;

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto fold1 = val1.fold(ctx, args);
			auto fold2 = val2.fold(ctx, args);

			auto type1 = fold1.getType();
			auto type2 = fold2.getType();

			if (type1 == Type::constant && type2 == Type::constant) {
				return ConstantBase{.value = fold1.getConstantValue() - fold2.getConstantValue()};
			} else if (type1 == Type::constant && type2 == Type::none) {
				auto constValue = fold1.getConstantValue();
				if (constValue == 0) return Monomial{.value = fold2}.mult(-1);
				//a - b = b * -1 + a
				return Monomial{.value = fold2}.mult(-1).add(constValue);
			} else if (type2 == Type::constant && type1 == Type::none) {
				auto constValue = fold2.getConstantValue();
				if (constValue == 0) return fold1;
				return SumMonomial{.value = fold1}.subtract(constValue);
			} else if (type1 == Type::constant && (type2 == Type::monomial || type2 == Type::summonomial || type2 == Type::prodmonomial)) {
				//a - b = b * -1 + a
				return fold2.mult(-1).add(fold1.getConstantValue());
			} else if (type2 == Type::constant && (type1 == Type::monomial || type1 == Type::summonomial || type1 == Type::prodmonomial)) {
				return fold1.subtract(fold2.getConstantValue());
			} else if (type1 == Type::summonomial && type2 == Type::summonomial) {
				auto mono1 = fold1.getMonomialValue();
				auto mono2 = fold2.getMonomialValue();

				return SumMonomial{.value = Difference<decltype(mono1), decltype(mono2)>{.val1 = mono1, .val2 = mono2}.fold(ctx, args)}
					.add(fold1.getSumParam() - fold2.getSumParam());
			}

			return Difference<decltype(fold1), decltype(fold2)>{
				.val1 = fold1,
				.val2 = fold2,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			if (val2.eval(context) == 0.f) return val1.print(context, prevStep);

			auto p1 = val1.print(context, Step::addition);
			auto p2 = val2.print(context, Step::addition);

			if (prevStep == Step::multiplication || prevStep == Step::division) {
				return fmt::format("({} - {})", p1, p2);
			}
			return fmt::format("{} - {}", p1, p2);
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			return val1.eval(context) - val2.eval(context);
		}
	};
}// namespace Formula