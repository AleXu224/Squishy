#pragma once

#include <concepts>
#include <utility>

// #include "context.hpp"
#include "type.hpp"


#ifdef GENSHIN
#include "formula/genshinConcepts.hpp"
#endif

namespace Formula {
	template<class T>
	concept GeneralFormulaConcept = requires(T t) {
		typename T::RetType;
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<typename T::RetType>;
		{ t.getType() } -> std::convertible_to<Type>;
	};

	template<class T, class V>
	concept FormulaConcept = requires(T t, V v) {
		requires std::same_as<typename T::RetType, V>;
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<V>;
		{ t.getType() } -> std::convertible_to<Type>;
	};

	template<class T>
	concept FloatFormula = FormulaConcept<T, float>;
	template<class T>
	concept BoolFormula = FormulaConcept<T, bool>;
	template<class T>
	concept IntFormula = FormulaConcept<T, int32_t>;

	template<class T>
	concept FormulaLike = GeneralFormulaConcept<T> || AdditionalFormulaLike<T>;
	template<class T>
	concept ArithmeticFormula = FloatFormula<T> || IntFormula<T> || AdditionalArithmeticFormula<T>;

	template<class T, class V>
	using SumType = decltype(std::declval<T>() + std::declval<V>());
	template<class T, class V>
	using DifferenceType = decltype(std::declval<T>() - std::declval<V>());
	template<class T, class V>
	using ProductType = decltype(std::declval<T>() * std::declval<V>());
	template<class T, class V>
	using FractionType = decltype(std::declval<T>() / std::declval<V>());
}// namespace Formula