#pragma once

#include "formula/base.hpp"


namespace Formula {
	template<FloatFormula T>
	struct Prefix : FormulaBase<float> {
		std::string_view prefix;
		T val;

		[[nodiscard]] FloatNode fold(const Context &context, const FoldArgs &args) const {
			return val.fold(context, args);
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return std::format("{} {}", prefix, Utils::formatFloat(val.eval(context)));
		}

		[[nodiscard]] float eval(const Context &context) const {
			return val.eval(context);
		}
	};
}// namespace Formula