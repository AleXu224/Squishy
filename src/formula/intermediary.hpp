#pragma once

#include "misc/element.hpp"
#include "step.hpp"
#include "utility"
#include "utils/optional.hpp"
#include <string>


namespace Stats {
	struct Loadout;
	struct Team;
}// namespace Stats

namespace Formula {
	template<class T>
	concept FloatFormula = requires(T t) {
		{ t.print(std::declval<const Stats::Loadout &>(), std::declval<const Stats::Team &>(), std::declval<Step>()) } -> std::same_as<std::string>;
		{ t.eval(std::declval<const Stats::Loadout &>(), std::declval<const Stats::Team &>()) } -> std::same_as<float>;
	};
	template<class T>
	concept BoolFormula = requires(T t) {
		{ t.print(std::declval<const Stats::Loadout &>(), std::declval<const Stats::Team &>(), std::declval<Step>()) } -> std::same_as<std::string>;
		{ t.eval(std::declval<const Stats::Loadout &>(), std::declval<const Stats::Team &>()) } -> std::same_as<bool>;
	};
	template<class T>
	concept ElementFormula = requires(T t) {
		{ t.print(std::declval<const Stats::Loadout &>(), std::declval<const Stats::Team &>(), std::declval<Step>()) } -> std::same_as<std::string>;
		{ t.eval(std::declval<const Stats::Loadout &>(), std::declval<const Stats::Team &>()) } -> std::same_as<Utils::JankyOptional<Misc::Element>>;
	};

	template<class T>
	concept FormulaLike = FloatFormula<T> || BoolFormula<T>;
}// namespace Formula