#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"

namespace Formula::Compiled {
	template<ArithmeticFormula T>
	struct Monomial {
		T value;
		using RetType = std::remove_cvref_t<decltype(std::declval<T>().eval(std::declval<const Formula::Context &>()))>;
		RetType sumParam = 0.f;
		RetType multParam = 1.f;

		[[nodiscard]] Monomial add(RetType value) const {
			Monomial ret = *this;
			ret.sumParam += value;
			return ret;
		}

		[[nodiscard]] Monomial subtract(RetType value) const {
			Monomial ret = *this;
			ret.sumParam -= value;
			return ret;
		}

		[[nodiscard]] Monomial mult(RetType value) const {
			Monomial ret = *this;
			ret.sumParam *= value;
			ret.multParam *= value;
			return ret;
		}

		[[nodiscard]] Monomial divide(RetType value) const {
			Monomial ret = *this;
			ret.sumParam /= value;
			ret.multParam /= value;
			return ret;
		}

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return sumParam + multParam * value.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return value.isConstant();
		}
	};

	template<ArithmeticFormula T>
	struct SumMonomial {
		T value;
		using RetType = std::remove_cvref_t<decltype(std::declval<T>().eval(std::declval<const Formula::Context &>()))>;
		RetType sumParam = 0.f;

		[[nodiscard]] SumMonomial add(RetType value) const {
			SumMonomial ret = *this;
			ret.sumParam += value;
			return ret;
		}

		[[nodiscard]] SumMonomial subtract(RetType value) const {
			SumMonomial ret = *this;
			ret.sumParam -= value;
			return ret;
		}

		[[nodiscard]] Monomial<T> mult(RetType value) const {
			auto ret = Monomial<T>{this->value};
			ret = ret.add(sumParam);
			ret = ret.mult(value);
			return ret;
		}

		[[nodiscard]] Monomial<T> divide(RetType value) const {
			auto ret = Monomial<T>{this->value};
			ret = ret.add(sumParam);
			ret = ret.divide(value);
			return ret;
		}

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return sumParam + value.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return value.isConstant();
		}
	};

	template<ArithmeticFormula T>
	struct ProdMonomial {
		T value;
		using RetType = std::remove_cvref_t<decltype(std::declval<T>().eval(std::declval<const Formula::Context &>()))>;
		RetType multParam = 1.f;

		Monomial<T> add(RetType value) const {
			auto ret = Monomial<T>{this->value};
			ret = ret.mult(multParam);
			ret = ret.add(value);
			return ret;
		}

		Monomial<T> subtract(RetType value) const {
			auto ret = Monomial<T>{this->value};
			ret = ret.mult(multParam);
			ret = ret.subtract(value);
			return ret;
		}

		ProdMonomial mult(RetType value) const {
			ProdMonomial ret = *this;
			ret.multParam *= value;
			return ret;
		}

		ProdMonomial divide(RetType value) const {
			ProdMonomial ret = *this;
			ret.multParam /= value;
			return ret;
		}

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return multParam * value.eval(context);
		}

		[[nodiscard]] bool isConstant() const {
			return value.isConstant();
		}
	};
}// namespace Formula::Compiled