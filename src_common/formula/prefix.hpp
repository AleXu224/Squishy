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

		void print(Descriptor &descriptor, const Context &context, Step) const {
			descriptor.pushPrefix(std::format("{} ", prefix));
			val.print(descriptor, context, Step::none);
		}

		[[nodiscard]] float eval(const Context &context) const {
			return val.eval(context);
		}
	};
}// namespace Formula