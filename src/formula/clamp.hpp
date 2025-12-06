#pragma once

#include "algorithm"
#include "compiled/constant.hpp"
#include "compiled/max.hpp"
#include "compiled/min.hpp"
#include "fmt/core.h"
#include "formula/intermediary.hpp"

namespace Formula {
	template<ArithmeticFormula T>
	struct Clamp {
		using RetType = FormulaType<T>;
		T val1;
		RetType min = 0.f;
		RetType max = 1.f;
		bool isPercentage = true;

		[[nodiscard]] auto compile(const Context &context) const {
			return Compiled::MaxMaker(
				Compiled::Constant<RetType>{.value = min}.wrap(),
				Compiled::MinMaker(
					Compiled::Constant<RetType>{.value = max}.wrap(),
					val1.compile(context)
				)
			);
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

	template<ArithmeticFormula T>
	struct Min {
		using RetType = FormulaType<T>;
		T val1;
		RetType val2;
		bool isPercentage = true;

		[[nodiscard]] auto compile(const Context &context) const {
			return Compiled::MinMaker(
				val1.compile(context),
				Compiled::Constant<RetType>{.value = val2}.wrap()
			);
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			auto val = val1.eval(context);
			if (val < val2) return val1.print(context, prevStep);
			return fmt::format("min({}, {})", val1.print(context, Step::none), val2);
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			return std::min(val1.eval(context), val2);
		}
	};

	template<ArithmeticFormula T>
	struct Max {
		using RetType = FormulaType<T>;
		T val1;
		RetType val2;
		bool isPercentage = true;

		[[nodiscard]] auto compile(const Context &context) const {
			return Compiled::MaxMaker(
				val1.compile(context),
				Compiled::Constant<RetType>{.value = val2}.wrap()
			);
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			auto val = val1.eval(context);
			if (val > val2) return val1.print(context, prevStep);
			return fmt::format("max({}, {})", val1.print(context, Step::none), val2);
		}

		[[nodiscard]] RetType eval(const Context &context) const {
			return std::max(val1.eval(context), val2);
		}
	};
}// namespace Formula