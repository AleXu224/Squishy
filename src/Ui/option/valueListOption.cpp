#include "valueListOption.hpp"

#include "store.hpp"

#include "Ui/utils/decodeModsSheet.hpp"

#include "ranges"
#include "vector"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/dropdownButton.hpp"

using namespace squi;

squi::core::Child UI::ValueListOption::State::build(const Element &element) {
	auto value = widget->option.getValue();

	auto mods = decodeOption(widget->option, widget->ctx);
	auto borderRadiusFunc = [hasMods = !mods.empty()](Button::Theme style) {
		if (hasMods) {
			style.resting.borderRadius = style.resting.borderRadius.withBottom(0.f);
			style.hovered.borderRadius = style.hovered.borderRadius.withBottom(0.f);
			style.active.borderRadius = style.active.borderRadius.withBottom(0.f);
			style.disabled.borderRadius = style.disabled.borderRadius.withBottom(0.f);
		}
		return style;
	};

	auto accentStyle = Button::Theme::Accent();

	return Column{
		.widget = widget->widget,
		.children{
			DropdownButton{
				.widget{
					.width = Size::Expand,
					.height = Size::Shrink,
					.sizeConstraints = BoxConstraints{
						.minHeight = 32.f,
					},
				},
				.theme = value.has_value() ? borderRadiusFunc(accentStyle) : borderRadiusFunc(Button::Theme::Standard()),
				.text = value.has_value()//
						  ? std::format("{}: {}", widget->option.prefix, value.value())
						  : std::format("{}: Not Active", widget->option.prefix),
				.items = [&]() {
					std::vector<ContextMenu::Item> ret{
						ContextMenu::Button{
							.text = "Not Active",
							.callback = [this]() {
								setState([&]() {
									widget->option.currentIndex = std::nullopt;
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
						},
						ContextMenu::Divider{},
					};

					for (const auto &[index, item]: std::views::enumerate(widget->option.values)) {
						ret.emplace_back(ContextMenu::Button{
							.text = std::format("{}", item),
							.callback = [this, index]() {
								setState([&]() {
									widget->option.currentIndex = index;
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
						});
					}

					return ret;
				}(),
			},
			!mods.empty()//
				? Box{
					  .color = Color::css(0x0, 0.3f),
					  .borderRadius = BorderRadius::Bottom(4.f),
					  .child = Column{
						  .widget{
							  .padding = 4.f,
						  },
						  .children = mods,
					  },
				  }
				: Child{},
		},
	};
}
