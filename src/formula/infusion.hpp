#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"
#include "misc/element.hpp"
#include "utils/optional.hpp"


namespace Formula {
	struct Infusion {
		::Misc::Element element;

		[[nodiscard]] inline std::string print(const Stats::Loadout &, const Stats::Team &, Step) const {
			return fmt::format("{}", Utils::Stringify(element));
		}

		[[nodiscard]] inline Utils::JankyOptional<Misc::Element> eval(const Stats::Loadout &, const Stats::Team &) const {
			return element;
		}
	};

	struct NoInfusion {
		[[nodiscard]] static inline std::string print(const Stats::Loadout &, const Stats::Team &, Step) {
			return "None";
		}

		[[nodiscard]] static inline Utils::JankyOptional<Misc::Element> eval(const Stats::Loadout &, const Stats::Team &) {
			return {};
		}
	};
}// namespace Formula