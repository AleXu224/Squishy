#pragma once

#include "formula/base.hpp"
#include "misc/element.hpp"
#include "utils/optional.hpp"


namespace Formula {
	struct Infusion : FormulaBase<Utils::JankyOptional<Misc::Element>, Type::constant> {
		::Misc::Element element;

		[[nodiscard]] std::string print(const Context &, Step) const {
			return fmt::format("{}", Utils::Stringify(element));
		}

		[[nodiscard]] Utils::JankyOptional<Misc::Element> eval(const Context &) const {
			return element;
		}
	};

	struct NoInfusion : FormulaBase<Utils::JankyOptional<Misc::Element>, Type::constant> {
		[[nodiscard]] static std::string print(const Context &, Step) {
			return "None";
		}

		[[nodiscard]] static Utils::JankyOptional<Misc::Element> eval(const Context &) {
			return {};
		}
	};
}// namespace Formula