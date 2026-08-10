#pragma once

#include "UI/elementToColor.hpp"
#include "formula/base.hpp"
#include "misc/element.hpp"
#include "utils/optional.hpp"


namespace Formula {
	struct Infusion : FormulaBase<Utils::JankyOptional<Misc::Element>, Type::constant> {
		::Misc::Element element;

		void print(Descriptor &descriptor, const Context &, Step) const {
			// descriptor.add(Style{.color = Utils::elementToColor(element), .text = Utils::Stringify(element)});
			descriptor.addName(Style{
				.color = Utils::elementToColor(element),
				.text = std::format("{}", Utils::Stringify(element)),
			});
		}

		[[nodiscard]] Utils::JankyOptional<Misc::Element> eval(const Context &) const {
			return element;
		}
	};

	struct NoInfusion : FormulaBase<Utils::JankyOptional<Misc::Element>, Type::constant> {
		static void print(Descriptor &descriptor, const Context &, Step) {
			descriptor.addName("None");
		}

		[[nodiscard]] static Utils::JankyOptional<Misc::Element> eval(const Context &) {
			return {};
		}
	};
}// namespace Formula