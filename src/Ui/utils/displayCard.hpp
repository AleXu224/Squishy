#pragma once

#include "Ui/utils/skillHeader.hpp"
#include "card.hpp"
#include "core/core.hpp"
#include "widgets/box.hpp"
#include "widgets/column.hpp"

namespace UI {
	using namespace squi;
	struct DisplayCard : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		std::optional<squi::Color> borderColor{};
		std::string title;
		std::string subtitle{};
		squi::Children children{};
		squi::Children footer{};

		[[nodiscard]] Child build(const Element &) const {
			auto newWidget = widget;
			newWidget.padding = newWidget.padding.value_or(1.f);

			return UI::Card{
				.widget = newWidget,
				.borderColor = borderColor,
				.child = Column{
					.children{
						UI::SkillHeader{
							.name = title,
							.subtitle = subtitle,
						},
						squi::Column{
							.widget{
								.height = squi::Size::Shrink,
							},
							.children = children,
						},
						footer.empty()
							? Child{}
							: Box{
								  .widget{
									  .padding = 4.f,
								  },
								  .color = squi::Color::css(0xFFFFFF, 0.1f),
								  .borderRadius = squi::BorderRadius::Bottom(7.f),
								  .child = squi::Column{
									  .widget{
										  .height = squi::Size::Shrink,
									  },
									  .spacing = 4.f,
									  .children = footer,
								  },
							  },
					},
				},
			};
		}
	};
}// namespace UI