#include "comboList.hpp"

#include "button.hpp"
#include "column.hpp"
#include "dialog.hpp"
#include "expander.hpp"


using namespace squi;

UI::ComboList::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	VoidObservable closeEvent{};

	return Dialog{
		.closeEvent = closeEvent,
		.title = "Edit combos",
		.content = Column{
			.children{
				Expander{},
			},
		},
		.buttons{
			Button{
				.text = "Close",
				.onClick = [closeEvent](GestureDetector::Event) {
					closeEvent.notify();
				},
			},
		},
	};
}
