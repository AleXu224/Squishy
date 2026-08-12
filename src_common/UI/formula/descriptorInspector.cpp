#include "descriptorInspector.hpp"

#include "theme.hpp"
#include "widgets/box.hpp"
#include "widgets/column.hpp"
#include "widgets/dialog.hpp"
#include "widgets/expander.hpp"

#include <iterator>

namespace UI {
	Child DescriptorInspector::State::buildFormula(const Formula::Descriptor &descriptor, bool firstRun) {
		auto theme = Theme::of(this);

		return Box{
			.widget{
				.height = Size::Shrink,
				.padding = firstRun ? Padding{16.f} : Padding{12.f},
			},
			.color = firstRun ? theme.accent * 0.1f : Color::black * 0.1f,
			.borderColor = firstRun ? theme.accent : Color::black * 0.2f,
			.borderWidth = BorderWidth{1.f}.withLeft(3.f),
			.borderRadius = 4.f,
			.borderPosition = Box::BorderPosition::inset,
			.child = RichText{
				.text = descriptor.spans | std::ranges::to<std::vector>(),
				.lineWrap = true,
			},
		};
	}

	squi::core::Children DescriptorInspector::State::buildItems(const Element &element, const Formula::Descriptor &descriptor) {
		Children items;

		for (const auto &parameter: descriptor.parameters) {
			Children contentItems;
			auto nestedItems = buildItems(element, parameter.formula);
			if (!nestedItems.empty()) {
				contentItems.emplace_back(buildFormula(parameter.formula));
				contentItems.insert(
					contentItems.end(),
					std::make_move_iterator(nestedItems.begin()),
					std::make_move_iterator(nestedItems.end())
				);
			}

			items.emplace_back(Expander{
				.title = RichText{
					.text{
						parameter.name,
						std::format(" {}", parameter.value.value),
					},
					.lineWrap = true,
				},
				.subtitle = RichText{
					.text{
						parameter.formula.spans | std::ranges::to<std::vector>(),
					},
					.fontSize = 12.f,
					.lineWrap = true,
				},
				.content = nestedItems.empty()//
							 ? Child{}
							 : Column{
								   .widget{.padding = Padding{12.f}.withRight(4.f)},
								   .spacing = 8.f,
								   .children = std::move(contentItems),
							   },
			});
		}

		return items;
	}

	squi::core::Child DescriptorInspector::State::build(const Element &element) {
		return Dialog{
			.widget = widget->widget,
			.width = 800.f,
			.title = "Calculation inspector",
			.content = Column{
				.spacing = 8.f,
				.children{
					buildFormula(widget->descriptor, true),
					Column{
						.spacing = 8.f,
						.children = buildItems(element, widget->descriptor),
					},
				},
			},
		};
	}
}// namespace UI
