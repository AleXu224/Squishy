#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	struct Constant {
		float value;

		[[nodiscard]] inline std::string print(const Stats::Sheet &, Step) const {
			return fmt::format("{:.2f}%", value * 100.f);
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &) const {
			return value;
		}
	};
	struct ConstantFlat {
		float value;

		[[nodiscard]] inline std::string print(const Stats::Sheet &, Step) const {
			return fmt::format("{:.2f}", value);
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &) const {
			return value;
		}
	};
}// namespace Formula