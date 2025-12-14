#include "statDisplay.hpp"
#include "Ui/elementToColor.hpp"
#include "skillEntry.hpp"

using namespace squi;

[[nodiscard]] squi::core::Child UI::StatDisplay::build(const Element &) const {
	if (!stat.stat.has_value()) return Child{};
	return SkillEntry{
		.isTransparent = isTransparent,
		.name = Utils::Stringify(stat.stat.value()),
		.value = stat.value,
		.color = Utils::elementToColor(Misc::Element::physical),
		.isPercentage = Utils::isPercentage(stat.stat),
	};
}
