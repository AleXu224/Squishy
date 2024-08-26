#pragma once

#include "intermediary.hpp"

namespace Formula {
	template<FloatFormula T>
	struct ZeroFilter {
		T val;

		[[nodiscard]] inline std::string print(const Context &context, Step prevStep) const {
			if (val.eval(context) == 0.f) return "";
			return val.print(context, prevStep);
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			return val.eval(context);
		}
	};
}// namespace Formula