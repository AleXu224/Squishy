#pragma once

#include "format"
#include "utils/stringify.hpp"

namespace Formula {
	[[nodiscard]] inline std::string Percentage(const std::string &name, float value, bool isPercentage) {
		return std::format("{}{}{}{}", name, (name.empty() ? "" : " "), Utils::formatFloat(isPercentage ? value * 100.f : value), (isPercentage ? "%" : ""));
	};
}// namespace Formula