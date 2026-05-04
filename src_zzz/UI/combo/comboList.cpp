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
		Agent::InstanceKey agentKey;
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
										.agentKey = widget->agentKey,
										.ctx = widget->ctx,
										.onSave = [this](Combo::Combo newCombo) {
											setState([&]() {
												widget->combo = newCombo;
											});
											widget->combo.updateEvent.notify();
											::Store::agents.at(widget->agentKey).updateEvent.notify();
										},
									});
								},
								.child = "Edit",
							},
							Button{
								.theme = Button::Theme::Standard(),
								.onClick = [this]() {
									auto &agent = ::Store::agents.at(widget->agentKey);
									auto newCombo = widget->combo;
									newCombo.name += " (Copy)";
									::Store::lastComboId++;
									newCombo.instanceKey.key = ::Store::lastComboId;
									agent.combos.insert(
										{
											::Store::lastComboId,
											std::move(newCombo),
										}
									);
									agent.updateEvent.notify();
								},
								.child = "Duplicate",
							},
							Button{
								.onClick = [this]() {
									auto &agent = ::Store::agents.at(widget->agentKey);
									auto &combos = agent.combos;

									combos.erase(widget->comboKey);
									agent.optimizationOptions->removeComboIfSelected(widget->comboKey);
									agent.updateEvent.notify();
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
						auto &agent = ::Store::agents.at(widget->agentKey);
						setState([&]() {
							::Store::lastComboId++;
							agent.combos.insert(
								{
									::Store::lastComboId,
									Combo::Combo{
										.instanceKey{::Store::lastComboId},
										.name = "New combo",
									},
								}
							);
						});
						agent.updateEvent.notify();
					},
					.child = "Add combo",
				},
				Column{
					.spacing = 4.f,
					.children = [&]() {
						Children ret;

						auto &agent = ::Store::agents.at(widget->agentKey);
						auto &combos = agent.combos;

						for (auto &[key, combo]: combos) {
							ret.emplace_back(ComboListEntry{
								.comboKey = key,
								.combo = combo,
								.agentKey = widget->agentKey,
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
