#pragma once

#include "formula/context.hpp"// IWYU pragma: keep
#include "formula/type.hpp"
#include "misc/attribute.hpp"

namespace Formula {
	template<class T, class V>
	concept FormulaConceptZZZ = requires(T t, V v) {
		requires std::same_as<typename std::remove_cvref_t<T>::RetType, V>;
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<V>;
		{ t.getType() } -> std::convertible_to<Type>;
	};

	template<class T>
	concept AdditionalFormulaLike = false;
	template<class T>
	concept AdditionalArithmeticFormula = false;


	template<class T>
	concept AttributeFormula = FormulaConceptZZZ<T, Misc::Attribute>;
}// namespace Formula