#include "toggleOption.hpp"
#include "Ui/utils/decodeModsSheet.hpp"
#include "store.hpp"
#include "theme.hpp"
#include "widgets/box.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/fontIcon.hpp"
#include "widgets/row.hpp"

using namespace squi;

struct ToggleBox : StatelessWidget {
	// Args
	Key key;
	Args widget{};
	bool active = false;

	[[nodiscard]] Child build(const Element &) const {
		auto accent = ThemeManager::getTheme().accent;

		return Box{
			.widget{
				.width = 20.f,
				.height = 20.f,
			},
			.color = active ? accent : Color{0.f, 0.f, 0.f, 0.1f},
			.borderColor{1.f, 1.f, 1.f, 0.60},
			.borderWidth = active ? 0.f : 1.f,
			.borderRadius{4.f},
			.borderPosition = squi::Box::BorderPosition::outset,
			.child = active ? FontIcon{.color{0.f, 0.f, 0.f, 1.0f}, .icon = 0xe5ca} : Child{},
		};
	}
};
squi::core::Child UI::ToggleOption::State::build(const Element &element) {
	auto mods = decodeOption(widget->option, widget->ctx);
	auto hasMods = !mods.empty();

	return Box{
		.widget = widget->widget,
		.color = hasMods ? Color::css(0x0, 0.3f) : Color::transparent,
		.borderRadius = 4.f,
		.child = Column{
			.children{
				Button{
					.widget{
						.width = Size::Expand,
					},
					.theme = Button::Theme::Subtle(),
					.onClick = [this]() {
						setState([&]() {
							widget->option.active = !widget->option.active;
						});
						std::visit(
							Utils::overloaded{
								[](const Character::InstanceKey &key) {
									::Store::characters.at(key).updateEvent.notify();
								},
								[](const Team::InstanceKey &key) {
									auto &team = ::Store::teams.at(key);
									for (const auto &character: team.stats.characters) {
										if (!character) continue;
										character->updateEvent.notify();
									}
								},
							},
							widget->instanceKey
						);
					},
					.child = Row{
						.crossAxisAlignment = Row::Alignment::center,
						.spacing = 8.f,
						.children{
							ToggleBox{
								.active = widget->option.active,
							},
							Container{
								.widget{.height = Size::Shrink},
								.child = Text{
									.text = std::string(widget->option.name),
									.lineWrap = true,
								},
							},
						},
					},
				},
				hasMods//
					? Column{
						  .widget{
							  .padding = 4.f,
						  },
						  .children = mods,
					  }
					: Child{},
			},
		},
	};
}
