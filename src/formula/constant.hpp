#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	struct Constant {
		float value;

		[[nodiscard]] inline std::string print(const Stats::Loadout &, const Stats::Team &, Step) const {
			return fmt::format("{:.2f}%", value * 100.f);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &, const Stats::Team &) const {
			return value;
		}
	};
	struct ConstantFlat {
		float value;

		[[nodiscard]] inline std::string print(const Stats::Loadout &, const Stats::Team &, Step) const {
			return fmt::format("{:.2f}", value);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &, const Stats::Team &) const {
			return value;
		}
	};
	struct ConstantBool {
		bool value;

		[[nodiscard]] inline std::string print(const Stats::Loadout &, const Stats::Team &, Step) const {
			return fmt::format("{}", value);
		}

		[[nodiscard]] inline bool eval(const Stats::Loadout &, const Stats::Team &) const {
			return value;
		}
	};
}// namespace Formula