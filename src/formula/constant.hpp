#pragma once

#include "fmt/core.h"
#include "formula/formulaContext.hpp"
#include "step.hpp"


namespace Formula {
	struct Constant {
		float value;

		[[nodiscard]] inline std::string print(const Context &, Step) const {
			return fmt::format("{:.1f}%", value * 100.f);
		}

		[[nodiscard]] inline float eval(const Context &) const {
			return value;
		}
	};
	struct ConstantFlat {
		float value;

		[[nodiscard]] inline std::string print(const Context &, Step) const {
			return fmt::format("{:.1f}", value);
		}

		[[nodiscard]] inline float eval(const Context &) const {
			return value;
		}
	};
	struct ConstantBool {
		bool value;

		[[nodiscard]] inline std::string print(const Context &, Step) const {
			return fmt::format("{}", value);
		}

		[[nodiscard]] inline bool eval(const Context &) const {
			return value;
		}
	};
}// namespace Formula