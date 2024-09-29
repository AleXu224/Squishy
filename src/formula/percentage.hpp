#pragma once

#include "fmt/core.h"


namespace Formula {
	[[nodiscard]] inline std::string Percentage(const std::string &name, float value, bool isPercentage) {
		return fmt::format("{}{}{:.1f}{}", name, (name.empty() ? "" : " "), value * (isPercentage ? 100.f : 1.f), (isPercentage ? "%" : ""));
	};
}// namespace Formula