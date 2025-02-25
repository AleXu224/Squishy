#include "combos.hpp"
#include "Ui/utils/displayCard.hpp"
#include "button.hpp"
#include "comboList.hpp"

using namespace squi;

UI::Combos::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return DisplayCard{
		.title = "Combos",
		.footer{
			Button{
				.text = "Edit",
				.onClick = [](GestureDetector::Event event) {
					event.widget.addOverlay(ComboList{});
				},
			},
		},
	};
}
