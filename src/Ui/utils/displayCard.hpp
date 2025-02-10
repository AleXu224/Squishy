#pragma once

#include "Ui/utils/skillHeader.hpp"
#include "box.hpp"
#include "card.hpp"

#include "column.hpp"
#include "widget.hpp"


namespace UI {
	struct DisplayCard {
		// Args
		squi::Widget::Args widget{};
		std::string_view title;
		std::string_view subtitle{};
		squi::Children children{};
		squi::Children footer{};

		operator squi::Child() const {
			return UI::Card{
				.widget = widget.withDefaultPadding(1.f),
				.child = squi::Column{
					.children{
						UI::SkillHeader{
							.name = title,
							.subtitle = subtitle,
						},
						squi::Column{
							.children = children,
						},
						footer.empty()
							? squi::Child{}
							: squi::Box{
								  .widget{
									  .padding = 4.f,
								  },
								  .color = squi::Color::css(0xFFFFFF, 0.1f),
								  .borderRadius = squi::BorderRadius::Bottom(7.f),
								  .child = squi::Column{
									  .widget{
										  .height = squi::Size::Shrink,
									  },
									  .children = footer,
								  },
							  },
					},
				},
			};
		}
	};
}// namespace UI