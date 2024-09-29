#pragma once

#include "formula/formulaContext.hpp"
#include "fmt/core.h"
#include "misc/element.hpp"
#include "step.hpp"
#include "utils/optional.hpp"


namespace Formula {
	struct Infusion {
		::Misc::Element element;

		[[nodiscard]] std::string print(const Context &, Step) const {
			return fmt::format("{}", Utils::Stringify(element));
		}

		[[nodiscard]] Utils::JankyOptional<Misc::Element> eval(const Context &) const {
			return element;
		}
	};

	struct NoInfusion {
		[[nodiscard]] static std::string print(const Context &, Step) {
			return "None";
		}

		[[nodiscard]] static Utils::JankyOptional<Misc::Element> eval(const Context &) {
			return {};
		}
	};
}// namespace Formula