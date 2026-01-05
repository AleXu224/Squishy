#include "statDisplay.hpp"
#include "Ui/elementToColor.hpp"
#include "skillEntry.hpp"
#include "widgets/container.hpp"

using namespace squi;

[[nodiscard]] squi::core::Child UI::StatDisplay::build(const Element &) const {
	if (!stat.stat.has_value()) return Container{};
	auto name = Utils::Stringify(stat.stat.value());
	if (!stat.activated) name = std::format("{} (Inactive)", name);
	return SkillEntry{
		.isTransparent = isTransparent,
		.name = name,
		.value = stat.value,
		.color = stat.activated ? Utils::elementToColor(Misc::Element::physical) : Color::gray,
		.isPercentage = Utils::isPercentage(stat.stat),
	};
}
