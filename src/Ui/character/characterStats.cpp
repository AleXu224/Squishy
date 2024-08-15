#include "characterStats.hpp"

#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/tooltip.hpp"
#include "Ui/utils/trueFalse.hpp"
#include "store.hpp"

#include <numeric>

using namespace squi;
UI::CharacterStats::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return UI::DisplayCard{
		.title = "Stats",
		.children = [characterKey = characterKey]() {
			auto &character = Store::characters.at(characterKey);
			Children ret{};
			std::array displayStats{Stats::characterDisplayStats, std::vector{Stats::fromElement(character.stats.character.base.element)}};

			Children ret2{};

			for (auto [stat, transparent]: std::views::zip(std::views::join(displayStats), Utils::trueFalse)) {
				ret2.emplace_back(UI::Tooltip{
					.message = [&]() {
						std::vector<std::string> a{};
						auto &modifiers = character.stats.character.sheet.fromStat(stat).modifiers;
						for (auto &modifier: modifiers) {
							if (!modifier.hasValue()) continue;
							a.emplace_back(modifier.print(character.stats));
						}
						return std::accumulate(
							a.begin(), a.end(),
							std::string(),
							[&](const std::string &val1, const std::string &val2) {
								return val1 + ((val1.empty() || val2.empty()) ? "" : " + ") + val2;
							}
						);
					}(),
					.child = UI::StatDisplay{
						.isTransparent = transparent,
						.stat{
							.stat = stat,
							.value = character.stats.character.sheet.fromStat(stat).getTotal(character.stats),
						},
					},
				});
			}
			ret.emplace_back(Column{
				.widget{
					.padding = Padding{4.f},
				},
				.children = ret2,
			});
			return ret;
		}(),
	};
}
