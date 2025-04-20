#pragma once

#include "compiled/constant.hpp"
#include "fmt/core.h"
#include "formula/formulaContext.hpp"
#include "step.hpp"


namespace Formula {
	struct Constant {
		float value;

		[[nodiscard]] inline auto compile(const Context &context) const {
			return Compiled::ConstantFloat(value);
		}

		[[nodiscard]] std::string print(const Context &, Step) const {
			return fmt::format("{:.1f}%", value * 100.f);
		}

		[[nodiscard]] float eval(const Context &) const {
			return value;
		}
	};
	struct ConstantFlat {
		float value;

		[[nodiscard]] inline auto compile(const Context &context) const {
			return Compiled::ConstantFloat(value);
		}

		[[nodiscard]] std::string print(const Context &, Step) const {
			return fmt::format("{:.1f}", value);
		}

		[[nodiscard]] float eval(const Context &) const {
			return value;
		}
	};
	struct ConstantBool {
		bool value;

		[[nodiscard]] inline auto compile(const Context &context) const {
			return Compiled::ConstantBool(value);
		}

		[[nodiscard]] std::string print(const Context &, Step) const {
			return fmt::format("{}", value);
		}

		[[nodiscard]] bool eval(const Context &) const {
			return value;
		}
	};
	struct ConstantInt {
		uint32_t value;

		[[nodiscard]] inline auto compile(const Context &context) const {
			return Compiled::ConstantInt(value);
		}

		[[nodiscard]] std::string print(const Context &, Step) const {
			return fmt::format("{}", value);
		}

		[[nodiscard]] uint32_t eval(const Context &) const {
			return value;
		}
	};
}// namespace Formula