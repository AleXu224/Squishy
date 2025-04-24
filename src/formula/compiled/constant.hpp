#pragma once

#include "formula/formulaContext.hpp"
#include "formulaBase.hpp"
#include "misc/element.hpp"
#include "utils/optional.hpp"


namespace Formula::Compiled {
	template<class T>
	struct Constant : FormulaBase<T, Type::constant> {
		T value;

		[[nodiscard]] T eval(const Formula::Context &context) const {
			return value;
		}

		[[nodiscard]] std::string print() const {
			if constexpr (std::is_same_v<T, Utils::JankyOptional<Misc::Element>>) {
				if (value.has_value()) {
					return fmt::format("{}", Utils::Stringify(value.value()));
				} else {
					return "No element";
				}
			} else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, int32_t> || std::is_same_v<T, bool>) {
				return fmt::format("Constant<{}>", value);
			} else {
				return "Unformattable";
			}
		}
	};

	using ConstantFloat = Constant<float>;
	using ConstantInt = Constant<int32_t>;
	using ConstantBool = Constant<bool>;
	using ConstantElement = Constant<Utils::JankyOptional<Misc::Element>>;
}// namespace Formula::Compiled