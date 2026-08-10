#pragma once

#include "base.hpp"

namespace Formula {
	template<ArithmeticFormula T>
	struct Monomial : FormulaBase<typename T::RetType, Type::monomial> {
		T value;
		using RetType = std::remove_cvref_t<decltype(std::declval<T>().eval(std::declval<const Formula::Context &>()))>;
		mutable RetType sumParam = 0;
		mutable RetType multParam = 1;

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto folded = value.fold(ctx, args);
			auto foldedType = folded.getType();
			switch (foldedType) {
				case Type::monomial:
				case Type::summonomial:
				case Type::prodmonomial:
					return folded.mult(multParam).add(sumParam);
				case Type::constant:
					return ConstantBase<RetType>{.value = sumParam + multParam * folded.getConstantValue()};
				default:
					break;
			}
			return Monomial<decltype(folded)>{
				.value = folded,
				.sumParam = sumParam,
				.multParam = multParam,
			};
		}

		[[nodiscard]] const Monomial &add(RetType value) const {
			sumParam += value;
			return *this;
		}

		[[nodiscard]] const Monomial &subtract(RetType value) const {
			sumParam -= value;
			return *this;
		}

		[[nodiscard]] const Monomial &mult(RetType value) const {
			sumParam *= value;
			multParam *= value;
			return *this;
		}

		[[nodiscard]] const Monomial &divide(RetType value) const {
			sumParam /= value;
			multParam /= value;
			return *this;
		}

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return sumParam + multParam * value.eval(context);
		}

		void print(Descriptor &descriptor, const Context &context, Step prevStep) const {
			const bool parens = prevStep == Step::multiplication || prevStep == Step::division;
			if (parens) descriptor.addName("(");
			descriptor.addValue(std::format("{}", sumParam));
			descriptor.addName(" + ");
			descriptor.addValue(std::format("{}", multParam));
			descriptor.addName(" * ");
			value.print(descriptor, context, prevStep);
			if (parens) descriptor.addName(")");
		}
	};

	template<ArithmeticFormula T>
	struct SumMonomial : FormulaBase<typename T::RetType, Type::summonomial> {
		T value;
		using RetType = std::remove_cvref_t<decltype(std::declval<T>().eval(std::declval<const Formula::Context &>()))>;
		mutable RetType sumParam = 0;

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto folded = value.fold(ctx, args);
			auto foldedType = folded.getType();
			switch (foldedType) {
				case Type::monomial:
				case Type::summonomial:
				case Type::prodmonomial:
					return folded.add(sumParam);
				case Type::constant:
					return ConstantBase<RetType>{.value = sumParam + folded.getConstantValue()};
				default:
					break;
			}
			return SumMonomial<decltype(folded)>{
				.value = folded,
				.sumParam = sumParam,
			};
		}

		[[nodiscard]] const SumMonomial &add(RetType value) const {
			sumParam += value;
			return *this;
		}

		[[nodiscard]] const SumMonomial &subtract(RetType value) const {
			sumParam -= value;
			return *this;
		}

		[[nodiscard]] Monomial<T> mult(RetType value) const {
			auto ret = Monomial<T>{.value = std::move(this->value)};
			ret = ret.add(sumParam);
			ret = ret.mult(value);
			return ret;
		}

		[[nodiscard]] Monomial<T> divide(RetType value) const {
			auto ret = Monomial<T>{.value = std::move(this->value)};
			ret = ret.add(sumParam);
			ret = ret.divide(value);
			return ret;
		}

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return sumParam + value.eval(context);
		}

		void print(Descriptor &descriptor, const Context &context, Step prevStep) const {
			const bool parens = prevStep == Step::multiplication || prevStep == Step::division;
			if (parens) descriptor.addName("(");
			descriptor.addValue(std::format("{}", sumParam));
			descriptor.addName(" + ");
			value.print(descriptor, context, prevStep);
			if (parens) descriptor.addName(")");
		}
	};

	template<ArithmeticFormula T>
	struct ProdMonomial : FormulaBase<typename T::RetType, Type::prodmonomial> {
		T value;
		using RetType = std::remove_cvref_t<decltype(std::declval<T>().eval(std::declval<const Formula::Context &>()))>;
		mutable RetType multParam = 1;

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto folded = value.fold(ctx, args);
			auto foldedType = folded.getType();
			switch (foldedType) {
				case Type::monomial:
				case Type::summonomial:
				case Type::prodmonomial:
					return folded.mult(multParam);
				case Type::constant:
					return ConstantBase<RetType>{.value = multParam * folded.getConstantValue()};
				default:
					break;
			}
			return ProdMonomial<decltype(folded)>{
				.value = folded,
				.multParam = multParam,
			};
		}

		Monomial<T> add(RetType value) const {
			auto ret = Monomial<T>{.value = std::move(this->value)};
			ret = ret.mult(multParam);
			ret = ret.add(value);
			return ret;
		}

		Monomial<T> subtract(RetType value) const {
			auto ret = Monomial<T>{.value = std::move(this->value)};
			ret = ret.mult(multParam);
			ret = ret.subtract(value);
			return ret;
		}

		const ProdMonomial &mult(RetType value) const {
			multParam *= value;
			return *this;
		}

		const ProdMonomial &divide(RetType value) const {
			multParam /= value;
			return *this;
		}

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return multParam * value.eval(context);
		}

		void print(Descriptor &descriptor, const Context &context, Step prevStep) const {
			descriptor.addValue(std::format("{}", multParam));
			descriptor.addName(" * ");
			value.print(descriptor, context, prevStep);
		}
	};
}// namespace Formula