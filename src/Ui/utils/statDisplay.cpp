#include "statDisplay.hpp"
#include "Ui/elementToColor.hpp"
#include "skillEntry.hpp"

using namespace squi;

UI::StatDisplay::operator squi::Child() const {
	return SkillEntry{
		.isTransparent = isTransparent,
		.name = Utils::Stringify(stat.stat),
		.value = stat.value,
		.color = Utils::elementToColor(Misc::Element::physical),
		.isPercentage = Utils::isPercentage(stat.stat),
	};
}
