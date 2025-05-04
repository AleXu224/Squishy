#include "characterStats.hpp"

#include "Ui/elementToColor.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/tag.hpp"
#include "Ui/utils/tooltip.hpp"
#include "Ui/utils/trueFalse.hpp"

#include "align.hpp"
#include "button.hpp"
#include "character/data.hpp"
#include "characterEditor.hpp"
#include "characterLoadouts.hpp"
#include "formula/stat.hpp"
#include "store.hpp"

#include "image.hpp"
#include "row.hpp"
#include "stack.hpp"
#include "stats/loadout.hpp"
#include "theme.hpp"

using namespace squi;
UI::CharacterStats::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();
	auto theme = ThemeManager::getTheme();

	return UI::DisplayCard{
		.widget = widget,
		.borderColor = Utils::elementToColor(ctx.active.stats.base.element),
		.title = ctx.active.stats.data.name,
		.children = [&]() {
			const auto &loadout = ctx.source;
			Children ret{};
			ret.emplace_back(Stack{
				.widget{
					.height = Size::Wrap,
				},
				.children{
					Image{
						.fit = Image::Fit::contain,
						.image = ImageProvider::fromFile(fmt::format("assets/Characters/{}/card.png", ctx.active.stats.data.name)),
					},
					Align{
						.xAlign = 0.f,
						.yAlign = 1.f,
						.child = Row{
							.widget{
								.width = Size::Shrink,
								.height = Size::Shrink,
								.margin = 8.f,
							},
							.spacing = 4.f,
							.children{
								UI::Tag{.sourceStr = std::format("Lvl {}", loadout.stats.sheet.level)},
								UI::Tag{.sourceStr = std::format("C{}", loadout.stats.sheet.constellation)},
							},
						},
					},
				},
			});
			std::array displayStats{Stats::characterDisplayStats, std::vector{Stats::fromElement(loadout.stats.base.element)}};

			Children ret2{};

			for (const auto &[stat, transparent]: std::views::zip(std::views::join(displayStats), Utils::trueFalse)) {
				auto message = Formula::EvalStat(Modifiers::displayTotal(), stat, [&](auto &&val) {
					return val.print(ctx, Formula::Step::none);
				});
				auto value = Formula::EvalStat(Modifiers::displayTotal(), stat, [&](auto &&val) {
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
		.footer{
			Row{
				.spacing = 4.f,
				.children{
					Button{
						.widget{
							.width = Size::Expand,
						},
						.text = "Edit",
						.onClick = [characterKey = characterKey, theme](GestureDetector::Event event) {
							auto &character = ::Store::characters.at(characterKey);
							auto _ = ThemeManager::pushTheme(theme);
							event.widget.addOverlay(UI::CharacterEditor{
								.character = character,
								.onSubmit = [](const Character::Instance &character) {
									auto &instance = Store::characters.at(character.instanceKey);
									instance.state.stats.sheet = character.state.stats.sheet;
									instance.updateEvent.notify();
								},
							});
						},
					},
					Button{
						.widget{
							.width = Size::Expand,
						},
						.text = "Loadouts",
						.onClick = [characterKey = characterKey, theme](GestureDetector::Event event) {
							auto _ = ThemeManager::pushTheme(theme);
							event.widget.addOverlay(UI::CharacterLoadouts{
								.characterKey = characterKey,
							});
						},
					},
				},
			},
		},
	};
}
