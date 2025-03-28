#pragma once

#include "formula/shieldModified.hpp"
#include "modifiers/total/total.hpp"
#include "nodeData.hpp"
#include "string_view"


namespace Node {
	using namespace Formula::Operators;

	template<class Frm, class T = decltype(Formula::ShieldModifier{})>
	struct Shield {
		std::string_view name;
		Utils::JankyOptional<Misc::Element> element{};
		Frm formula;
		T modifier{};

		[[nodiscard]] static constexpr auto _getFormula(
			Frm formula,
			T modifier
		) {
			auto totalShield_ = Modifiers::total.shield_ + modifier.shield_;
			auto elementBonus = modifier.elementBonus;
			auto additiveHp = modifier.additiveHp;

			return (formula + additiveHp) * elementBonus * (1.f + totalShield_);
		}

		using _FormulaRetType = decltype(_getFormula(
			std::declval<Frm>(),
			std::declval<T>()
		));

		_FormulaRetType _formula = _getFormula(formula, modifier);

		Data _data = ShieldData{element};
	};
}// namespace Node