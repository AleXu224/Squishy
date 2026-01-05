#pragma once

#include "core/core.hpp"
#include "option/option.hpp"
#include "stats/sheet.hpp"


namespace UI {
	using namespace squi;
	[[nodiscard]] Children decodeModsSheet(const Stats::ModsSheet &sheet, const Formula::Context &ctx, bool *transparent = nullptr);
	[[nodiscard]] Children decodeOption(const Option::Types &option, const Formula::Context &ctx);
}// namespace UI