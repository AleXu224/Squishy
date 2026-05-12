#pragma once

#include "context.hpp"// IWYU pragma: keep
#include "fmt/core.h"
#include "formulaBase.hpp"
#include "step.hpp"
#include "utils/stringify.hpp"


namespace Formula {
	template<class T, bool percentage>
	struct ConstantBase : FormulaBase<T, Type::constant> {
		T value;

		[[nodiscard]] std::string print(const Context &, Step) const {
			if constexpr (std::is_same_v<T, float>) {
				if constexpr (percentage) {
					return fmt::format("{}%", Utils::formatFloat(value * 100.f));
				} else {
					return fmt::format("{}", Utils::formatFloat(value));
				}
			} else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, bool>) {
				return fmt::format("{}", value);
			} else {
				return fmt::format("Cannot format constant of type {}", typeid(T).name());
			}
		}

		[[nodiscard]] T eval(const Formula::Context &) const {
			return value;
		}
	};

	using Constant = ConstantBase<float, true>;
	using ConstantFlat = ConstantBase<float>;
	using ConstantInt = ConstantBase<int32_t>;
	using ConstantBool = ConstantBase<bool>;
}// namespace Formula