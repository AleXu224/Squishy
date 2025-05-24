#pragma once

#include "formula/formulaContext.hpp"
#include "formulaBase.hpp"// IWYU pragma: export
#include "misc/element.hpp"
#include "utility"
#include "utils/optional.hpp"


namespace Formula::Compiled {
	template<class T>
	concept FloatFormula = requires(T t) {
		requires std::same_as<typename T::Type, float>;
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<float>;
		{ t.getType() } -> std::same_as<Type>;
	};
	template<class T>
	concept BoolFormula = requires(T t) {
		requires std::same_as<typename T::Type, bool>;
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<bool>;
		{ t.getType() } -> std::same_as<Type>;
	};
	template<class T>
	concept IntFormula = requires(T t) {
		requires std::same_as<typename T::Type, int32_t>;
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<int32_t>;
		{ t.getType() } -> std::same_as<Type>;
	};
	template<class T>
	concept ElementFormula = requires(T t) {
		requires std::same_as<typename T::Type, Utils::JankyOptional<Misc::Element>>;
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<Utils::JankyOptional<Misc::Element>>;
		{ t.getType() } -> std::same_as<Type>;
	};

	template<class T>
	concept FormulaLike = FloatFormula<T> || BoolFormula<T> || IntFormula<T> || ElementFormula<T>;
	template<class T>
	concept ArithmeticFormula = FloatFormula<T> || IntFormula<T>;

	template<class T, class V>
	using SumType = decltype(std::declval<T>() + std::declval<V>());
	template<class T, class V>
	using DifferenceType = decltype(std::declval<T>() - std::declval<V>());
	template<class T, class V>
	using ProductType = decltype(std::declval<T>() * std::declval<V>());
	template<class T, class V>
	using FractionType = decltype(std::declval<T>() / std::declval<V>());

	template<class T>
	using FormulaType = decltype(std::declval<T>().eval(std::declval<const Context &>()));
}// namespace Formula::Compiled