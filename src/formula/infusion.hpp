#pragma once

#include "formula/formulaContext.hpp"
#include "fmt/core.h"
#include "misc/element.hpp"
#include "step.hpp"
#include "utils/optional.hpp"


namespace Formula {
	struct Infusion {
		::Misc::Element element;

		[[nodiscard]] inline std::string print(const Context &, Step) const {
			return fmt::format("{}", Utils::Stringify(element));
		}

		[[nodiscard]] inline Utils::JankyOptional<Misc::Element> eval(const Context &) const {
			return element;
		}
	};

	struct NoInfusion {
		[[nodiscard]] static inline std::string print(const Context &, Step) {
			return "None";
		}

		[[nodiscard]] static inline Utils::JankyOptional<Misc::Element> eval(const Context &) {
			return {};
		}
	};
}// namespace Formula