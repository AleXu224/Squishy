#pragma once

#include "constant.hpp"
#include "formula/formulaContext.hpp"
#include "misc/element.hpp"
#include "utility"
#include "utils/optional.hpp"


namespace Formula::Compiled {
	template<class T>
	concept MonomialFormula = requires(T t) {
		{ t.add(std::declval<typename T::RetType>()) };
		{ t.subtract(std::declval<typename T::RetType>()) };
		{ t.mult(std::declval<typename T::RetType>()) };
		{ t.divide(std::declval<typename T::RetType>()) };
		{ t.eval(std::declval<const Formula::Context &>()) };
	};
	template<class T>
	concept SumMonomialFormula = requires(T t) {
		{ t.sumParam };
	};
	template<class T>
	concept ProdMonomialFormula = requires(T t) {
		{ t.multParam };
	};

	template<class T>
	concept SumOnlyMonomial = MonomialFormula<std::remove_cvref_t<T>> && !ProdMonomialFormula<std::remove_cvref_t<T>>;
	template<class T>
	concept ProdOnlyMonomial = MonomialFormula<std::remove_cvref_t<T>> && !SumMonomialFormula<std::remove_cvref_t<T>>;

	template<class T>
	concept FloatFormula = requires(T t) {
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<float>;
		{ t.isConstant() } -> std::same_as<bool>;
	};
	template<class T>
	concept BoolFormula = requires(T t) {
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<bool>;
		{ t.isConstant() } -> std::same_as<bool>;
	};
	template<class T>
	concept IntFormula = requires(T t) {
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<uint32_t>;
		{ t.isConstant() } -> std::same_as<bool>;
	};
	template<class T>
	concept ElementFormula = requires(T t) {
		{ t.eval(std::declval<const Formula::Context &>()) } -> std::same_as<Utils::JankyOptional<Misc::Element>>;
		{ t.isConstant() } -> std::same_as<bool>;
	};

	template<class T>
	struct is_constant_helper : std::false_type {};
	template<class U>
	struct is_constant_helper<Constant<U>> : std::true_type {};

	template<class T>
	concept ConstantFormula = is_constant_helper<T>::value;
	template<class T>
	concept ArithmeticConstant = std::same_as<T, ConstantFloat> || std::same_as<T, ConstantInt>;

	template<class T>
	concept FormulaLike = FloatFormula<T> || BoolFormula<T> || IntFormula<T> || ElementFormula<T>;
	template<class T>
	concept ArithmeticFormula = FloatFormula<T> || IntFormula<T>;
	template<class T>
	concept ArithmeticVariableFormula = ArithmeticFormula<T> && !ArithmeticConstant<T>;
	template<class T>
	concept ArithmeticNotMonomial = ArithmeticFormula<T> && !MonomialFormula<T>;
	template<class T>
	concept ArithmeticVariableNotMonomial = ArithmeticFormula<T> && !MonomialFormula<T> && !ConstantFormula<T>;

	template<class T, class V>
	concept OneVariable = !ConstantFormula<std::remove_cvref_t<T>> || !ConstantFormula<std::remove_cvref_t<V>>;
	template<class T, class V>
	concept OnlyVariable = !ConstantFormula<T> && !ConstantFormula<V>;
	template<class T, class V>
	concept OneVariableNoMonomial = OneVariable<T, V> && ((!ConstantFormula<std::remove_cvref_t<T>> && !ConstantFormula<std::remove_cvref_t<V>>) || (!MonomialFormula<T> && !MonomialFormula<V>) );

	template<class T>
	using FormulaType = decltype(std::declval<T>().eval(std::declval<const Context &>()));
}// namespace Formula::Compiled