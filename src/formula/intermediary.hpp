#pragma once

#include "formula/formulaContext.hpp"
#include "misc/element.hpp"
#include "step.hpp"
#include "utility"
#include "utils/optional.hpp"
#include <string>


namespace Formula {
	template<class T>
	concept FloatFormula = requires(T t) {
		{ t.print(std::declval<const Formula::Context &>(), std::declval<Step>()) } -> std::same_as<std::string>;
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<float>;
	};
	template<class T>
	concept BoolFormula = requires(T t) {
		{ t.print(std::declval<const Formula::Context &>(), std::declval<Step>()) } -> std::same_as<std::string>;
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<bool>;
	};
	template<class T>
	concept IntFormula = requires(T t) {
		{ t.print(std::declval<const Formula::Context &>(), std::declval<Step>()) } -> std::same_as<std::string>;
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<uint32_t>;
	};
	template<class T>
	concept ElementFormula = requires(T t) {
		{ t.print(std::declval<const Formula::Context &>(), std::declval<Step>()) } -> std::same_as<std::string>;
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<Utils::JankyOptional<Misc::Element>>;
	};

	template<class T>
	concept FormulaLike = FloatFormula<T> || BoolFormula<T> || IntFormula<T> || ElementFormula<T>;
	template<class T>
	concept ArithmeticFormula = FloatFormula<T> || IntFormula<T>;

	template<class T>
	using FormulaType = decltype(std::declval<T>().eval(std::declval<const Context &>()));
}// namespace Formula