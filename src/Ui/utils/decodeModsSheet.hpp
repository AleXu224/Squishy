#pragma once

#include "option/option.hpp"
#include "stats/sheet.hpp"
#include "widgetArgs.hpp"


namespace UI {
	[[nodiscard]] squi::Children decodeModsSheet(Stats::ModsSheet &sheet, const Formula::Context &ctx);
	[[nodiscard]] squi::Children decodeOption(const Option::Types &option, const Formula::Context &ctx);
}// namespace UI