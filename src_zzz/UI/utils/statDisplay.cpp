#include "statDisplay.hpp"

#include "UI/attributeToColor.hpp"
#include "UI/utils/skillEntry.hpp"
#include "utils/overloaded.hpp"
#include "widgets/container.hpp"

using namespace squi;

[[nodiscard]] squi::core::Child UI::StatDisplay::build(const Element &) const {
	auto [stat, activated, value] = std::visit(
		Utils::overloaded{
			[](const StatValue &val) {
				return std::make_tuple(val.stat, true, val.value);
			},
			[this](const DiscSubstat &val) {
				return std::make_tuple(val.stat, val.activated, val.getValue(rarity));
			},
		},
		this->stat
	);

	if (!stat.has_value()) return Container{};
	auto name = Utils::Stringify(stat.value());
	if (!activated) name = std::format("{} (Inactive)", name);
	std::optional<std::string> suffix{};
	if (showRolls) {
		float rolls = 0.f;
		if (auto it = Stats::Values::subStat.find(stat.value()); it != Stats::Values::subStat.end()) {
			rolls = std::round(value / it->second.at(rarity)) - 1.f;
		}
		if (rolls >= 1) {
			suffix = std::make_optional<std::string>(std::format("+{:.0f}", rolls));
		}
	}
	return SkillEntry{
		.isTransparent = isTransparent,
		.name = name,
		.nameSuffix = suffix,
		.value = value,
		.color = activated ? Color::white : Color::gray,
		.nameSuffixColor = Utils::attributeToColor(Misc::Attribute::physical),
		.isPercentage = Utils::isPercentage(stat),
	};
}
