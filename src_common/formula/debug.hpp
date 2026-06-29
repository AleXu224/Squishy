#pragma once

#include "formula/base.hpp"


namespace Formula {
	template<FormulaLike T>
	struct Debug : FormulaBase<float> {
		T formula;

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			return formula.fold(context, args);
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			return formula.print(context, prevStep);
		}

		[[nodiscard]] float eval(const Context &context) const {
			return formula.eval(context);
		}
	};
}// namespace Formula