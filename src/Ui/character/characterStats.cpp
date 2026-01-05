#include "characterStats.hpp"

#include "Ui/elementToColor.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/tag.hpp"
#include "Ui/utils/trueFalse.hpp"

#include "character/data.hpp"
#include "characterEditor.hpp"
#include "characterLoadouts.hpp"
#include "store.hpp"

#include "modifiers/total/total.hpp"
#include "stats/loadout.hpp"
#include "widgets/button.hpp"
#include "widgets/image.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/stack.hpp"
#include "widgets/tooltip.hpp"


using namespace squi;
squi::core::Child UI::CharacterStats::State::build(const Element &element) {
	return UI::DisplayCard{
		.widget = widget->widget,
		.borderColor = Utils::elementToColor(widget->ctx.active.stats.base.element),
		.title = std::string(widget->ctx.active.stats.data.name),
		.children = [&]() {
			const auto &loadout = widget->ctx.source;
			Children ret{};
			ret.emplace_back(Stack{
				.widget{
					.height = Size::Wrap,
				},
				.children{
					Image{
						.fit = Image::Fit::contain,
						.image = ImageProvider::fromFile(fmt::format("assets/Characters/{}/card.png", widget->ctx.active.stats.data.name)),
					},
					Row{
						.widget{
							.width = Size::Shrink,
							.height = Size::Shrink,
							.alignment = Alignment::BottomLeft,
							.margin = 8.f,
						},
						.spacing = 4.f,
						.children{
							UI::Tag{.sourceStr = std::format("Lvl {}", loadout.stats.sheet.level)},
							UI::Tag{.sourceStr = std::format("C{}", loadout.stats.sheet.constellation)},
						},
					},
				},
			});
			std::array displayStats{Stats::characterDisplayStats, std::vector{Stats::fromElement(loadout.stats.base.element)}};

			Children ret2{};

			for (const auto &[stat, transparent]: std::views::zip(std::views::join(displayStats), Utils::trueFalse)) {
				auto formula = Stats::fromStat(Modifiers::displayTotal(), stat);
				auto message = formula.print(widget->ctx);
				auto value = formula.eval(widget->ctx);
				ret2.emplace_back(Tooltip{
					.text = message,
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
		.footer{
			Row{
				.spacing = 4.f,
				.children{
					Button{
						.widget{
							.width = Size::Expand,
						},
						.onClick = [this]() {
							auto &character = ::Store::characters.at(widget->characterKey);
							Navigator::of(this).pushOverlay(UI::CharacterEditor{
								.character = character,
								.onSubmit = [](const Character::Instance &character) {
									auto &instance = Store::characters.at(character.instanceKey);
									instance.state.stats.sheet = character.state.stats.sheet;
									instance.updateEvent.notify();
								},
							});
						},
						.child = "Edit",
					},
					Button{
						.widget{
							.width = Size::Expand,
						},
						.onClick = [this]() {
							Navigator::of(this).pushOverlay(UI::CharacterLoadouts{
								.characterKey = widget->characterKey,
							});
						},
						.child = "Loadouts",
					},
				},
			},
		},
	};
}
