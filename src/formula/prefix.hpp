#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<FloatFormula T>
	struct Prefix {
		std::string_view prefix;
		T val;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{} {}", prefix, val.eval(context));
		}

		[[nodiscard]] float eval(const Context &context) const {
			return val.eval(context);
		}
	};
}// namespace Formula