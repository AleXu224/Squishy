#pragma once

#include <concepts>
#include <utility>

#include "formula/context.hpp"// IWYU pragma: keep
#include "formula/type.hpp"
#include "misc/element.hpp"
#include "utils/optional.hpp"


namespace Formula {
	template<class T, class V>
	concept FormulaConceptGenshin = requires(T t, V v) {
		requires std::same_as<typename T::Type, V>;
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<V>;
		{ t.getType() } -> std::convertible_to<Type>;
	};

	template<class T>
	concept AdditionalFormulaLike = FormulaConceptGenshin<T, Utils::JankyOptional<Misc::Element>>;
	template<class T>
	concept AdditionalArithmeticFormula = false;
}// namespace Formula