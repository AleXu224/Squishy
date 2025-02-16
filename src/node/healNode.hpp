#pragma once

#include "formula/healModifier.hpp"
#include "modifiers/total/total.hpp"
#include "nodeData.hpp"
#include "string_view"


namespace Node {
	using namespace Formula::Operators;

	template<class Frm, class T = decltype(Formula::HealModifier{})>
	struct Heal {
		std::string_view name;
		Frm formula;
		T modifier{};

		[[nodiscard]] static constexpr auto _getFormula(
			Frm formula,
			T modifier
		) {
			auto totalHb = Modifiers::total.hb + modifier.hb;
			auto totalIncHb = Modifiers::total.incHb + modifier.incHb;

			auto bonus = Formula::Constant(1.f) + totalHb + totalIncHb;

			return formula
				 * bonus;
		}

		using _FormulaRetType = decltype(_getFormula(
			std::declval<Frm>(),
			std::declval<T>()
		));

		_FormulaRetType _formula = _getFormula(formula, modifier);

		Data _data = HealData{};
	};
}// namespace Node