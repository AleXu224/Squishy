#include "UI/DropdownButton.hpp"
#include "box.hpp"
#include "contextMenu.hpp"
#include "fontIcon.hpp"
#include "gestureDetector.hpp"
#include "row.hpp"
#include "text.hpp"
#include "window.hpp"
#include <debugapi.h>
#include <stdio.h>

using namespace squi;
using namespace Squishy;

DropdownButton::operator Child() const {
	auto storage = std::make_shared<Storage>(style);

	return GestureDetector{
		.onClick = [](Widget &w, auto& gd){
			auto window = dynamic_cast<Window *>(w.state.root);
			window->addOverlay(ContextMenu{
				.position = w.getRect().getTopLeft(),
				.items{
					ContextMenu::Item{
						.text = "Hello",
					},
					ContextMenu::Item{
						.text = "World",
					},
				},
			});
		},
		.onUpdate = [](auto&, auto& gd) {},
		.child = Box{
			.widget = widget
						  .withPadding({0.f, 12.f, 0.f, 12.f})
						  .withHeight(32.f)
						  .withWidth(Size::Shrink),
			.color = Color::RGBA(1, 1, 1, 0.06),
			.borderColor = Color::RGBA(1, 1, 1, 0.09),
			.borderWidth = 1.f,
			.borderRadius = 4.f,
			.borderPosition = Box::BorderPosition::outset,
			.child = Row{
				.widget{
					.width = Size::Shrink,
					// .onLayout = [](auto&, auto, auto){
					//     DebugBreak();
					// },
				},
				.alignment = Row::Alignment::center,
				.spacing = 4.f,
				.children{
					Text{
						.text = "Hello",
					},
					FontIcon{
						.icon = "\uE972",
						.size = 12.f,
					},
				},
			},
		},
	};
}