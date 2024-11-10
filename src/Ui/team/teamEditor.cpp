#include "teamEditor.hpp"

#include "Ui/character/characterSelector.hpp"
#include "Ui/utils/editorItem.hpp"
#include "Ui/utils/tooltip.hpp"
#include "align.hpp"
#include "button.hpp"
#include "character/data.hpp"
#include "character/instance.hpp"
#include "character/key.hpp"
#include "column.hpp"
#include "dialog.hpp"
#include "fontIcon.hpp"
#include "row.hpp"
#include "store.hpp"
#include "textBox.hpp"


using namespace squi;

UI::TeamEditor::operator squi::Child() const {
	auto storage = std::make_shared<Storage>(instance);

	VoidObservable closeEvent{};
	std::array<Observable<std::optional<Character::InstanceKey>>, 4> characterChangeEvent{};

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

	Child content = Column{
		.spacing = 16.f,
		.children{
			name,
		},
	};

	for (size_t i = 0; i < storage->team.stats.characters.size(); i++) {
		const auto &character = storage->team.stats.characters.at(i);

		auto changeCharacterButton = Button{
			.widget{
				.onInit = [characterChangeEvent, i, storage](Widget &w) {
					observe(w, characterChangeEvent.at(i), [&w, storage, i](std::optional<Character::InstanceKey> instanceKey) {
						storage->team.stats.characters.at(i) = instanceKey
																   .and_then([](auto &&val) -> std::optional<Character::Instance *> {
																	   return &Store::characters.at(val);
																   })
																   .value_or(nullptr);
						if (instanceKey) {
							Button::State::style.of(w) = ButtonStyle::Accent();
							Button::State::updateText.of(w).notify(std::string(Store::characters.at(instanceKey.value()).loadout.character.data.name));
						} else {
							Button::State::style.of(w) = ButtonStyle::Standard();
							Button::State::updateText.of(w).notify("None");
						}
					});
				},
			},
			.text = character ? character->loadout.character.data.name : "None",
			.style = character ? ButtonStyle::Accent() : ButtonStyle::Standard(),
			.onClick = [characterChangeEvent, i](GestureDetector::Event event) {
				event.widget.addOverlay(CharacterSelector{
					.onSelect = [characterChangeEvent, i](Character::InstanceKey instanceKey) {
						characterChangeEvent.at(i).notify(instanceKey);
					},
				});
			},
		};

		auto deleteButton = Tooltip{
			.message = "Remove character",
			.child = Button{
				.widget{
					.padding = 6.f,
					.onInit = [character, characterChangeEvent, i](Widget &w) {
						if (!character) w.flags.visible = false;
						observe(w, characterChangeEvent.at(i), [&w](std::optional<Character::InstanceKey> instanceKey) {
							w.flags.visible = instanceKey.has_value();
						});
					},
				},
				.style = ButtonStyle::Standard(),
				.onClick = [characterChangeEvent, i](GestureDetector::Event) {
					characterChangeEvent.at(i).notify(std::nullopt);
				},
				.child = Align{
					.child = FontIcon{
						.icon = 0xe5cd,
					},
				},
			},
		};

		content->addChild(
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
