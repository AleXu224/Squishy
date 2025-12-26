#include "comboList.hpp"

#include "comboEditor.hpp"
#include "store.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/dialog.hpp"
#include "widgets/expander.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"


using namespace squi;

namespace {
	struct ComboListEntry : StatefulWidget {
		// Args
		Key key;
		Combo::InstanceKey comboKey;
		Combo::Combo &combo;
		Character::InstanceKey characterKey;
		Formula::Context ctx;
		const Element &element;

		struct State : WidgetState<ComboListEntry> {
			VoidObserver comboModifiedEvent;

			void initState() override {
				comboModifiedEvent = widget->combo.updateEvent.observe([this]() {
					setState([&]() {});
				});
			}

			Child build(const Element &element) override {
				return Expander{
					.title = widget->combo.name,
					.action = Row{
						.widget{
							.width = Size::Shrink,
						},
						.crossAxisAlignment = Row::Alignment::center,
						.spacing = 4.f,
						.children{
							Button{
								.theme = Button::Theme::Standard(),
								.onClick = [this]() {
									Navigator::of(widget->element).pushOverlay(UI::ComboEditor{
										.combo = widget->combo,
										.characterKey = widget->characterKey,
										.ctx = widget->ctx,
										.onSave = [this](Combo::Combo newCombo) {
											setState([&]() {
												widget->combo = newCombo;
											});
											widget->combo.updateEvent.notify();
										},
									});
								},
								.child = "Edit combo",
							},
							Button{
								.onClick = [this]() {
									auto &character = ::Store::characters.at(widget->characterKey);
									auto &combos = character.combos;

									combos.erase(widget->comboKey);
									character.updateEvent.notify();
								},
								.child = "Delete",
							},
						},
					},
				};
			}
		};
	};
}// namespace

squi::core::Child UI::ComboList::State::build(const Element &element) {
	return Dialog{
		.closeEvent = closeEvent,
		.title = "Combo list",
		.content = Column{
			.spacing = 8.f,
			.children{
				Button{
					.onClick = [this]() {
						auto &character = ::Store::characters.at(widget->characterKey);
						setState([&]() {
							::Store::lastComboId++;
							character.combos.insert(
								{
									::Store::lastComboId,
									Combo::Combo{
										.instanceKey{::Store::lastComboId},
										.name = "New combo",
									},
								}
							);
						});
						character.updateEvent.notify();
					},
					.child = "Add combo",
				},
				Column{
					.spacing = 4.f,
					.children = [&]() {
						Children ret;

						auto &character = ::Store::characters.at(widget->characterKey);
						auto &combos = character.combos;

						for (auto &[key, combo]: combos) {
							ret.emplace_back(ComboListEntry{
								.combo = combo,
								.characterKey = widget->characterKey,
								.ctx = widget->ctx,
								.element = element,
							});
						}

						return ret;
					}(),
				},
			},
		},
		.buttons{
			Button{
				.widget{.width = Size::Expand},
				.theme = Button::Theme::Standard(),
				.onClick = [this]() {
					closeEvent.notify();
				},
				.child = "Close",
			},
		},
	};
}
