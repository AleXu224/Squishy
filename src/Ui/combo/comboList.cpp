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
			.children = [&]() {
				Children ret;

				auto &combos = ::Store::characters.at(characterKey).combos;

				for (auto &combo: combos) {
					ret.emplace_back(Expander{
						.heading = combo.name,
						.actions{
							Button{
								.text = "Edit combo",
								.onClick = [&combo, characterKey, ctx, combosModifiedEvent](GestureDetector::Event event) {
									event.widget.addOverlay(UI::ComboEditor{
										.combo = combo,
										.characterKey = characterKey,
										.ctx = ctx,
										.onSave = [&combo, combosModifiedEvent](Combo::Combo newCombo) {
											combo = newCombo;
											combosModifiedEvent.notify();
										},
									});
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
			.children{
				Button{
					.text = "Add combo",
					.onClick = [characterKey = characterKey, combosModifiedEvent](GestureDetector::Event event) {
						auto &character = ::Store::characters.at(characterKey);
						character.combos.emplace_back(Combo::Combo{
							.name = "New combo",
						});
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
