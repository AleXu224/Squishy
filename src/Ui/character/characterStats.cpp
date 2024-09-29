#include "characterStats.hpp"

#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/tooltip.hpp"
#include "Ui/utils/trueFalse.hpp"

#include "formula/stat.hpp"

#include "stats/loadout.hpp"

using namespace squi;
UI::CharacterStats::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return UI::DisplayCard{
		.title = "Stats",
		.children = [&]() {
			const auto &loadout = ctx.source;
			Children ret{};
			std::array displayStats{Stats::characterDisplayStats, std::vector{Stats::fromElement(loadout.character.base.element)}};

			Children ret2{};

			for (const auto &[stat, transparent]: std::views::zip(std::views::join(displayStats), Utils::trueFalse)) {
				auto message = Formula::EvalStat(Modifiers::displayTotal, stat, [&](auto &&val) {
					return val.print(ctx, Formula::Step::none);
				});
				auto value = Formula::EvalStat(Modifiers::displayTotal, stat, [&](auto &&val) {
					return val.eval(ctx);
				});
				ret2.emplace_back(UI::Tooltip{
					.message = message,
					.child = UI::StatDisplay{
						.isTransparent = transparent,
						.stat{
							.stat = stat,
							.value = value,
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
