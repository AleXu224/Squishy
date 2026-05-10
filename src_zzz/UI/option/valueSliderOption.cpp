#include "valueSliderOption.hpp"

#include "store.hpp"

#include "UI/utils/decodeModsSheet.hpp"

#include "vector"
#include "widgets/box.hpp"
#include "widgets/column.hpp"
#include "widgets/slider.hpp"
#include "widgets/text.hpp"

using namespace squi;

void UI::ValueSliderOption::State::initState() {
	mods = decodeOption(widget->option, widget->ctx);
}

void UI::ValueSliderOption::State::widgetUpdated() {
	mods = decodeOption(widget->option, widget->ctx);
}

squi::core::Child UI::ValueSliderOption::State::build(const Element &element) {
	auto value = widget->option.getValue();
	auto hasMods = !mods.empty();

	return Box{
		.widget = widget->widget,
		.color = hasMods ? Color::css(0x0, 0.3f) : Color::transparent,
		.borderRadius = 4.f,
		.child = Column{
			.children{
				Text{
					.widget{
						.margin = Margin{8.f}.withBottom(0.f),
					},
					.text = std::string(widget->option.name),
					.lineWrap = true,
				},
				Slider{
					.widget{
						.width = Size::Expand,
						.height = Size::Shrink,
						.sizeConstraints = BoxConstraints{
							.minHeight = 32.f,
						},
					},
					.minValue = widget->option.values.empty() ? 0.f : widget->option.values.front(),
					.maxValue = widget->option.values.empty() ? 1.f : widget->option.values.back(),
					.value = value,
					.ticks = widget->option.values,
					.onChange = [this](float newValue) {
						// Round to nearest value in options
						if (!widget->option.values.empty()) {
							auto it = std::lower_bound(widget->option.values.begin(), widget->option.values.end(), newValue);
							if (it == widget->option.values.end()) {
								newValue = widget->option.values.back();
							} else if (it != widget->option.values.begin()) {
								float lower = *(it - 1);
								float upper = *it;
								newValue = (std::abs(newValue - lower) < std::abs(newValue - upper)) ? lower : upper;
							}
						}
						setState([&]() {
							widget->option.value = newValue;
							if (widget->onChange) {
								widget->onChange(newValue);
							}
						});
						std::visit(
							Utils::overloaded{
								[](const Agent::InstanceKey &key) {
									::Store::agents.at(key).updateEvent.notify();
								},
								[](const Team::InstanceKey &key) {
									auto &team = ::Store::teams.at(key);
									for (const auto &agent: team.stats.agents) {
										if (!agent) continue;
										agent->updateEvent.notify();
									}
								},
							},
							widget->instanceKey
						);
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
