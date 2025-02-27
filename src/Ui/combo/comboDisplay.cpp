#include "comboDisplay.hpp"
#include "Ui/utils/displayCard.hpp"
#include "button.hpp"
#include "comboList.hpp"

using namespace squi;

UI::ComboDisplay::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return DisplayCard{
		.title = "Combos",
		.children = [](){
			Children ret;


			return ret;
		}(),
		.footer{
			Button{
				.text = "Edit",
				.onClick = [characterKey = characterKey, ctx = ctx](GestureDetector::Event event) {
					event.widget.addOverlay(ComboList{
						.characterKey = characterKey,
						.ctx = ctx,
					});
				},
			},
		},
	};
}
