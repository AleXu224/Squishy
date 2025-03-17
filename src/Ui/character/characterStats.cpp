#include "characterStats.hpp"

#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/statDisplay.hpp"
#include "Ui/utils/tooltip.hpp"
#include "Ui/utils/trueFalse.hpp"

#include "align.hpp"
#include "button.hpp"
#include "characterEditor.hpp"
#include "formula/stat.hpp"
#include "store.hpp"

#include "image.hpp"
#include "row.hpp"
#include "stack.hpp"
#include "stats/loadout.hpp"
#include "text.hpp"

using namespace squi;
UI::CharacterStats::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return UI::DisplayCard{
		.widget = widget,
		.title = ctx.active.character.data.name,
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
						.image = ImageProvider::fromFile(fmt::format("assets/Characters/{}/card.png", ctx.active.character.data.name)),
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
								Box{
									.widget{
										.width = Size::Shrink,
										.height = Size::Shrink,
										.padding = 4.f,
									},
									.borderRadius = 4.f,
									.child = Text{
										.text = std::format("Lvl {}", loadout.character.sheet.level),
										.color = Color::css(0x0, 1.f),
									},
								},
								Box{
									.widget{
										.width = Size::Shrink,
										.height = Size::Shrink,
										.padding = 4.f,
									},
									.borderRadius = 4.f,
									.child = Text{
										.text = std::format("C{}", loadout.character.sheet.constellation),
										.color = Color::css(0x0, 1.f),
									},
								},
							},
						},
					},
				},
			});
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
		.footer{
			Button{
				.widget{
					.width = Size::Expand,
				},
				.text = "Edit",
				.onClick = [characterKey = characterKey](GestureDetector::Event event) {
					auto &character = ::Store::characters.at(characterKey);
					event.widget.addOverlay(UI::CharacterEditor{
						.character = character,
						.onSubmit = [](const Character::Instance &character) {
							auto &instance = Store::characters.at(character.instanceKey);
							instance.loadout.character.sheet = character.loadout.character.sheet;
							instance.updateEvent.notify();
						},
					});
				},
			},
		},
	};
}
