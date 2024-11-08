#include "teamEditor.hpp"
#include "Ui/utils/editorItem.hpp"
#include "button.hpp"
#include "column.hpp"
#include "dialog.hpp"
#include "textBox.hpp"

using namespace squi;

UI::TeamEditor::operator squi::Child() const {
	auto storage = std::make_shared<Storage>(instance);

	VoidObservable closeEvent{};

	auto name = EditorItem{
		.name = "Name",
		.child = TextBox{
			.text = storage->team.name,
			.controller{
				.onChange = [storage](std::string_view newString) {
					storage->team.name = newString;
				},
			},
		},
	};

	auto content = Column{
		.spacing = 16.f,
		.children{
			name,
		},
	};

	auto footer = Children{
		Button{
			.widget{.width = Size::Expand},
			.text = "Save",
			.style = ButtonStyle::Accent(),
			.onClick = [closeEvent, storage, onSubmit = onSubmit](GestureDetector::Event) {
				if (onSubmit) onSubmit(storage->team);
				closeEvent.notify();
			},
		},
		Button{
			.widget{.width = Size::Expand},
			.text = "Cancel",
			.style = ButtonStyle::Standard(),
			.onClick = [closeEvent](GestureDetector::Event) {
				closeEvent.notify();
			},
		},
	};

	return Dialog{
		.closeEvent = closeEvent,
		.title = "Edit team",
		.content = content,
		.buttons = footer,
	};
}
