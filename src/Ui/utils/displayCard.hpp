#pragma once

#include "Ui/utils/skillHeader.hpp"
#include "card.hpp"

#include "column.hpp"
#include "widget.hpp"


namespace UI {
	struct DisplayCard {
		// Args
		squi::Widget::Args widget{};
		std::string_view title;
		squi::Children children{};

		operator squi::Child() const {
			return UI::Card{
				.widget{
					.padding = squi::Padding{1.f},
				},
				.child = squi::Column{
					.children{
						UI::SkillHeader{
							.name = title,
						},
						squi::Column{
							.children = children,
						},
					},
				},
			};
		}
	};
}// namespace UI