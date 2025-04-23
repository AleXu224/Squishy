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
	};

	using ConstantFloat = Constant<float>;
	using ConstantInt = Constant<int32_t>;
	using ConstantBool = Constant<bool>;
	using ConstantElement = Constant<Utils::JankyOptional<Misc::Element>>;
}// namespace Formula::Compiled