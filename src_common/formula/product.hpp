#pragma once

#include "base.hpp"
#include "monomial.hpp"

namespace Formula {
	template<ArithmeticFormula T, ArithmeticFormula U>
	struct Product : FormulaBase<ProductType<typename T::RetType, typename U::RetType>> {
		T val1;
		U val2;

		using RetType = ProductType<typename T::RetType, typename U::RetType>;
		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto fold1 = val1.fold(ctx, args);
			auto fold2 = val2.fold(ctx, args);

			auto type1 = fold1.getType();
			auto type2 = fold2.getType();

			if (type1 == Type::constant && type2 == Type::constant) {
				return ConstantBase{.value = fold1.getConstantValue() * fold2.getConstantValue()};
			} else if (type1 == Type::constant && type2 == Type::none) {
				auto constValue = fold1.getConstantValue();
				if (constValue == 0)
					return fold1;
				else if (constValue == 1)
					return fold2;
				return ProdMonomial{.value = fold2}.mult(constValue);
			} else if (type2 == Type::constant && type1 == Type::none) {
				auto constValue = fold2.getConstantValue();
				if (constValue == 0)
					return fold2;
				else if (constValue == 1)
					return fold1;
				return ProdMonomial{.value = fold1}.mult(constValue);
			} else if (type1 == Type::constant && (type2 == Type::monomial || type2 == Type::summonomial || type2 == Type::prodmonomial)) {
				auto constValue = fold1.getConstantValue();
				if (constValue == 0)
					return fold1;
				else if (constValue == 1)
					return fold2;
				return fold2.mult(constValue);
			} else if (type2 == Type::constant && (type1 == Type::monomial || type1 == Type::summonomial || type1 == Type::prodmonomial)) {
				auto constValue = fold2.getConstantValue();
				if (constValue == 0)
					return fold2;
				else if (constValue == 1)
					return fold1;
				return fold1.mult(constValue);
			} else if (type1 == Type::prodmonomial && type2 == Type::prodmonomial) {
				auto mono1 = fold1.getMonomialValue();
				auto mono2 = fold2.getMonomialValue();

				return ProdMonomial{.value = Product<decltype(mono1), decltype(mono2)>{.val1 = mono1, .val2 = mono2}.fold(ctx, args)}
					.mult(fold1.getMultParam() * fold2.getMultParam());
			}

			return Product<decltype(fold1), decltype(fold2)>{
				.val1 = fold1,
				.val2 = fold2,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			if (val1.eval(context) == 1.f) return val2.print(context, prevStep);
			if (val2.eval(context) == 1.f) return val1.print(context, prevStep);

			auto p1 = val1.print(context, Step::multiplication);
			auto p2 = val2.print(context, Step::multiplication);

			if (prevStep == Step::division) {
				return std::format("({} * {})", p1, p2);
			}

			return std::format("{} * {}", p1, p2);
		}

		[[nodiscard]] auto eval(const Context &context) const {
			return val1.eval(context) * val2.eval(context);
		}
	};
}// namespace Formula