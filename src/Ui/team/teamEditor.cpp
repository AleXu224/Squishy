#include "teamEditor.hpp"

#include "Ui/character/characterSelector.hpp"
#include "Ui/utils/editorItem.hpp"
#include "character/data.hpp"
#include "character/instance.hpp"
#include "character/key.hpp"
#include "store.hpp"
#include "widgets/column.hpp"
#include "widgets/dialog.hpp"
#include "widgets/fontIcon.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/textBox.hpp"
#include "widgets/tooltip.hpp"
#include "widgets/visibility.hpp"


using namespace squi;

squi::core::Child UI::TeamEditor::State::build(const Element &element) {
	auto name = EditorItem{
		.name = "Name",
		.child = TextBox{
			.controller = nameController,
		},
	};

	Children content{
		name
	};

	for (size_t i = 0; i < team.stats.characters.size(); i++) {
		const auto &character = team.stats.characters.at(i);

		Child changeCharacterButton = Button{
			.theme = character ? Button::Theme::Accent() : Button::Theme::Standard(),
			.onClick = [this, i]() {
				Navigator::of(this).pushOverlay(CharacterSelector{
					.onSelect = [this, i](Character::InstanceKey instanceKey) {
						auto character = Store::characters.find(instanceKey);
						if (character == Store::characters.end()) return;
						setState([&]() {
							team.stats.characters.at(i) = &character->second;
						});
					},
				});
			},
			.child = character ? std::string(character->state.stats.data.name) : "None",
		};

		auto deleteButton = Tooltip{
			.text = "Remove character",
			.child = Visibility{
				.visible = character != nullptr,
				.child = Button{
					.onClick = [this, i]() {
						setState([&]() {
							team.stats.characters.at(i) = nullptr;
						});
					},
					.child = FontIcon{
						.icon = 0xe5cd,
					},
				},
			},
		};

		content.emplace_back(
			EditorItem{
				.name = fmt::format("Character {}", i + 1),
				.child = Row{
					.widget{.width = Size::Shrink, .height = Size::Shrink},
					.spacing = 4.f,
					.children{
						changeCharacterButton,
						deleteButton,
					},
				},
			}
		);
	}

	auto footer = Children{
		Button{
			.widget{.width = Size::Expand},
			.theme = Button::Theme::Accent(),
			.onClick = [this]() {
				if (widget->onSubmit) widget->onSubmit(team);
				closeEvent.notify();
			},
			.child = "Save",
		},
		Button{
			.widget{.width = Size::Expand},
			.theme = Button::Theme::Standard(),
			.onClick = [this]() {
				closeEvent.notify();
			},
			.child = "Cancel",
		},
	};

	return Dialog{
		.closeEvent = closeEvent,
		.title = "Edit team",
		.content = Column{
			.spacing = 16.f,
			.children = content,
		},
		.buttons = footer,
	};
}
