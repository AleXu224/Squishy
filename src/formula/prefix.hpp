#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<FloatFormula T>
	struct Prefix {
		std::string_view prefix;
		T val;

		[[nodiscard]] auto compile(const Context &context) const {
			return val.compile(context);
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			// return fmt::format("{} {}", prefix, val.eval(context));
			return "";
		}

		[[nodiscard]] float eval(const Context &context) const {
			return val.eval(context);
		}
	};
}// namespace Formula