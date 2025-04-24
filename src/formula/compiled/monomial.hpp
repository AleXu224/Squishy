#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"

namespace Formula::Compiled {
	template<ArithmeticFormula T>
	struct Monomial : FormulaBase<typename T::Type, Type::monomial> {
		T value;
		using RetType = std::remove_cvref_t<decltype(std::declval<T>().eval(std::declval<const Formula::Context &>()))>;
		mutable RetType sumParam = 0.f;
		mutable RetType multParam = 1.f;

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

		[[nodiscard]] std::string print() const {
			return fmt::format("Monomial<{}, {}, {}>", sumParam, multParam, value.print());
		}
	};

	template<ArithmeticFormula T>
	struct SumMonomial : FormulaBase<typename T::Type, Type::summonomial> {
		T value;
		using RetType = std::remove_cvref_t<decltype(std::declval<T>().eval(std::declval<const Formula::Context &>()))>;
		mutable RetType sumParam = 0.f;

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

		[[nodiscard]] std::string print() const {
			return fmt::format("SumMonomial<{}, {}>", sumParam, value.print());
		}
	};

	template<ArithmeticFormula T>
	struct ProdMonomial : FormulaBase<typename T::Type, Type::prodmonomial> {
		T value;
		using RetType = std::remove_cvref_t<decltype(std::declval<T>().eval(std::declval<const Formula::Context &>()))>;
		mutable RetType multParam = 1.f;

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

		[[nodiscard]] std::string print() const {
			return fmt::format("ProdMonomial<{}, {}>", multParam, value.print());
		}
	};
}// namespace Formula::Compiled