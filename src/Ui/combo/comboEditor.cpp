#include "comboEditor.hpp"

#include "Ui/combo/nodePicker.hpp"
#include "button.hpp"
#include "column.hpp"
#include "dialog.hpp"
#include "dropdownButton.hpp"
#include "expander.hpp"
#include "reaction/list.hpp"
#include "rebuilder.hpp"
#include "row.hpp"
#include "textBox.hpp"


using namespace squi;

namespace {
	[[nodiscard]] std::string getReactionName(const std::variant<const Reaction::None *, const Reaction::Amplifying *, const Reaction::Additive *> &reaction) {
		return std::visit(
			Utils::overloaded{
				[](const Reaction::None *) {
					return std::string("No reaction");
				},
				[](auto &&reaction) {
					return std::string(reaction->name);
				},
			},
			reaction
		);
	}

	Child comboEditorEntries(std::shared_ptr<UI::ComboEditor::Storage> storage, Formula::Context ctx, VoidObservable nodeListChangedEvent) {
		Children ret;

		for (auto &entry: storage->combo.entries) {
			const auto &node = std::visit(
				[](auto &&val) {
					return val.resolve();
				},
				entry.source
			);

			std::optional<Misc::Element> element = std::nullopt;

			std::visit(
				Utils::overloaded{
					[&](const Node::AtkData &data) {
						element = Formula::getElement(
							data.source,
							data.element,
							ctx
						);
					},
					[](const Node::InfoData &data) {},
					[](const Node::HealData &data) {},
				},
				node.data
			);

			decltype(DropdownButton::textUpdater) textUpdater{};

			auto reactionName = getReactionName(Reaction::List::fromNodeReaction(entry.reaction));

			auto reactionSelector = DropdownButton{
				.style = ButtonStyle::Standard(),
				.text = reactionName,
				.items = [&]() {
					std::vector<ContextMenu::Item> ret;

					ret.emplace_back(ContextMenu::Item{
						.text = "No reaction",
						.content = [&entry, textUpdater]() {
							entry.reaction = Misc::NodeReaction::none;
							textUpdater.notify("No reaction");
						},
					});

					for (const auto &reaction: ::Reaction::List::amplifyingList) {
						if (reaction->trigger != element.value()) continue;
						ret.emplace_back(ContextMenu::Item{
							.text = std::string{reaction->name},
							.content = [&entry, reaction, textUpdater]() {
								entry.reaction = reaction->nodeReaction;
								textUpdater.notify(std::string{reaction->name});
							},
						});
					}
					for (const auto &reaction: ::Reaction::List::additiveList) {
						if (reaction->trigger != element.value()) continue;
						ret.emplace_back(ContextMenu::Item{
							.text = std::string{reaction->name},
							.content = [&entry, reaction, textUpdater]() {
								entry.reaction = reaction->nodeReaction;
								textUpdater.notify(std::string{reaction->name});
							},
						});
					}

					return ret;
				}(),
				.textUpdater = textUpdater,
			};

			auto deleteButton = Button{
				.text = "Delete",
				.onClick = [storage, entryPtr = &entry, nodeListChangedEvent](GestureDetector::Event event) {
					storage->combo.entries.remove_if([entryPtr, nodeListChangedEvent](const Combo::Entry &entry) {
						return &entry == entryPtr;
					});
					nodeListChangedEvent.notify();
				},
			};

			ret.emplace_back(Expander{
				.heading = node.name,
				.actions{
					reactionSelector,
					deleteButton,
				},
			});
		}

		return Column{
			.children = ret,
		};
	}
}// namespace

UI::ComboEditor::operator squi::Child() const {
	auto storage = std::make_shared<Storage>(combo);

	VoidObservable closeEvent{};
	VoidObservable nodeListChangedEvent{};

	return Dialog{
		.closeEvent = closeEvent,
		.title = "Edit combo",
		.content = Column{
			.spacing = 4.f,
			.children{
				TextBox{
					.text = storage->combo.name,
					.controller{
						.onChange = [storage](std::string_view newText) {
							storage->combo.name = newText;
						},
					},
				},
				Rebuilder{
					.rebuildEvent = nodeListChangedEvent,
					.buildFunc = std::bind(comboEditorEntries, storage, ctx, nodeListChangedEvent),
				},
				Row{
					.children{
						Button{
							.text = "Add node",
							.onClick = [characterKey = characterKey, storage, nodeListChangedEvent, ctx = ctx](GestureDetector::Event event) {
								event.widget.addOverlay(NodePicker{
									.characterKey = characterKey,
									.ctx = ctx,
									.onSelect = [storage, nodeListChangedEvent](Combo::Source::Types source) {
										storage->combo.entries.emplace_back(Combo::Entry{
											.multiplier = 1.f,
											.source = source,
										});
										nodeListChangedEvent.notify();
									},
								});
							},
						},
					},
				},
			},
		},
		.buttons{
			Button{
				.widget{.width = Size::Expand},
				.text = "Save",
				.style = ButtonStyle::Accent(),
				.onClick = [closeEvent, storage, onSave = onSave](GestureDetector::Event) {
					onSave(storage->combo);
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
		},
	};
}
