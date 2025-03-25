#include "comboList.hpp"

#include "button.hpp"
#include "column.hpp"
#include "comboEditor.hpp"
#include "container.hpp"
#include "dialog.hpp"
#include "expander.hpp"
#include "rebuilder.hpp"
#include "store.hpp"


using namespace squi;

namespace {
	[[nodiscard]] Child comboListBuilder(const Character::InstanceKey &characterKey, const Formula::Context &ctx, VoidObservable combosModifiedEvent) {
		return Column{
			.spacing = 4.f,
			.children = [&]() {
				Children ret;

				auto &character = ::Store::characters.at(characterKey);
				auto &combos = character.combos;

				for (auto &[key, combo]: combos) {
					ret.emplace_back(Expander{
						.heading = combo.name,
						.actions{
							Button{
								.text = "Edit combo",
								.style = ButtonStyle::Standard(),
								.onClick = [&combo, characterKey, ctx, combosModifiedEvent](GestureDetector::Event event) {
									event.widget.addOverlay(UI::ComboEditor{
										.combo = combo,
										.characterKey = characterKey,
										.ctx = ctx,
										.onSave = [&combo, combosModifiedEvent](Combo::Combo newCombo) {
											combo = newCombo;
											combo.updateEvent.notify();
											combosModifiedEvent.notify();
										},
									});
								},
							},
							Button{
								.text = "Delete",
								.onClick = [&combos, combosModifiedEvent, &character, key](auto) {
									combos.erase(key);
									combosModifiedEvent.notify();
									character.updateEvent.notify();
								},
							},
						},
					});
				}

				return ret;
			}(),
		};
	}
}// namespace

UI::ComboList::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	VoidObservable closeEvent{};
	VoidObservable combosModifiedEvent{};

	return Dialog{
		.closeEvent = closeEvent,
		.title = "Combo list",
		.content = Column{
			.spacing = 8.f,
			.children{
				Button{
					.text = "Add combo",
					.onClick = [characterKey = characterKey, combosModifiedEvent](GestureDetector::Event event) {
						auto &character = ::Store::characters.at(characterKey);
						::Store::lastComboId++;
						character.combos.insert(
							{
								::Store::lastComboId,
								Combo::Combo{
									.name = "New combo",
								},
							}
						);
						character.updateEvent.notify();
						combosModifiedEvent.notify();
					},
				},
				Rebuilder{
					.rebuildEvent = combosModifiedEvent,
					.buildFunc = std::bind(comboListBuilder, characterKey, ctx, combosModifiedEvent),
				}
			},
		},
		.buttons{
			Container{},
			Button{
				.widget{.width = Size::Expand},
				.text = "Close",
				.style = ButtonStyle::Standard(),
				.onClick = [closeEvent](GestureDetector::Event) {
					closeEvent.notify();
				},
			},
		},
	};
}
