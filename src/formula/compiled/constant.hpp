#pragma once

#include "formula/formulaContext.hpp"
#include "misc/element.hpp"
#include "utils/optional.hpp"


namespace Formula::Compiled {
	template<class T>
	struct Constant {
		T value;

		[[nodiscard]] T eval(const Formula::Context &context) const {
			return value;
		}

		[[nodiscard]] bool isConstant() const {
			return true;
		}
	};

	using ConstantFloat = Constant<float>;
	using ConstantInt = Constant<uint32_t>;
	using ConstantBool = Constant<bool>;
	using ConstantElement = Constant<Utils::JankyOptional<Misc::Element>>;
}// namespace Formula::Compiled