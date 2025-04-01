#include "comboEditor.hpp"

#include "Ui/combo/nodePicker.hpp"
#include "align.hpp"
#include "button.hpp"
#include "column.hpp"
#include "dialog.hpp"
#include "dropdownButton.hpp"
#include "expander.hpp"
#include "fontIcon.hpp"
#include "numberBox.hpp"
#include "optionPicker.hpp"
#include "reaction/list.hpp"
#include "rebuilder.hpp"
#include "row.hpp"
#include "text.hpp"
#include "textBox.hpp"
#include "utils/overloaded.hpp"


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

	Child comboEditorEntries(std::shared_ptr<UI::ComboEditor::Storage> storage, Formula::Context ctx, Character::InstanceKey characterKey, VoidObservable nodeListChangedEvent) {
		Children ret;

		for (auto it = storage->combo.entries.begin(); it != storage->combo.entries.end(); it++) {
			auto &entry = *it;
			const auto &node = std::visit(
				[&](auto &&val) {
					return val.resolve(entry.options);
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
					[](const Node::ShieldData &data) {},
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
						if (!element || reaction->trigger != element.value()) continue;
						ret.emplace_back(ContextMenu::Item{
							.text = std::string{reaction->name},
							.content = [&entry, reaction, textUpdater]() {
								entry.reaction = reaction->nodeReaction;
								textUpdater.notify(std::string{reaction->name});
							},
						});
					}
					for (const auto &reaction: ::Reaction::List::additiveList) {
						if (!element || reaction->trigger != element.value()) continue;
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

			auto moveUpButton = Button{
				.widget{
					.width = Size::Shrink,
					.padding = Padding(8.f, 6.f),
				},
				.style = ButtonStyle::Standard(),
				.disabled = &entry == &storage->combo.entries.front(),
				.onClick = [it, &entry, nodeListChangedEvent](GestureDetector::Event event) {
					std::swap(entry, *std::next(it, -1));
					nodeListChangedEvent.notify();
				},
				.child = Align{
					.child = FontIcon{.icon = 0xe316},
				},
			};
			auto moveDownButton = Button{
				.widget{
					.width = Size::Shrink,
					.padding = Padding(8.f, 6.f),
				},
				.style = ButtonStyle::Standard(),
				.disabled = &entry == &storage->combo.entries.back(),
				.onClick = [it, &entry, nodeListChangedEvent](GestureDetector::Event event) {
					std::swap(entry, *std::next(it, 1));
					nodeListChangedEvent.notify();
				},
				.child = Align{
					.child = FontIcon{.icon = 0xe313},
				},
			};

			auto multiplierBox = NumberBox{
				.value = entry.multiplier,
				.onChange = [&entry](float newVal) {
					entry.multiplier = newVal;
				},
			};


			auto captionStr = std::format("{:.1f}", node.formula.eval(ctx.withReaction(Reaction::List::fromNodeReaction(entry.reaction))));
			auto sourceStr = std::visit(
				Utils::overloaded{
					[](const Combo::Source::Character &source) -> std::string {
						return Utils::Stringify(source.slot);
					},
					[](const Combo::Source::Weapon &source) -> std::string {
						return "Weapon";
					},
					[](const Combo::Source::Artifact &source) -> std::string {
						return "Artifact";
					},
					[](const Combo::Source::Combo &source) -> std::string {
						return "Combo";
					},
				},
				entry.source
			);
			ret.emplace_back(Expander{
				.icon = Row{
					.widget{
						.width = Size::Shrink,
						.height = Size::Shrink,
					},
					.spacing = 4.f,
					.children{
						moveUpButton,
						moveDownButton,
						multiplierBox,
					},
				},
				.heading = Row{
					.alignment = Row::Alignment::center,
					.spacing = 8.f,
					.children{
						Text{
							.text = node.name,
							.lineWrap = true,
							.color = Node::getColor(node.data, ctx),
						},
						Box{
							.widget{
								.width = Size::Shrink,
								.padding = 4.f,
							},
							.color = Color::css(0xffffff, 0.8f),
							.borderRadius = 4.f,
							.child = Text{
								.text = sourceStr,
								.fontSize = 12.f,
								.color = Color::black,
							},
						},
					},
				},
				.caption = captionStr,
				.actions{
					reactionSelector.items.size() <= 1 ? Child{} : reactionSelector,
					deleteButton,
				},
				.expandedContent = Column{
					.children{
						Button{
							.onClick = [&](GestureDetector::Event event) {
								event.widget.addOverlay(UI::OptionPicker{
									.characterKey = characterKey,
									.ctx = ctx,
									.onSelect = [](Combo::Option) {},
								});
							},
						},
					},
				},
			});
		}

		return Column{
			.spacing = 4.f,
			.children = ret,
		};
	}
}// namespace

UI::ComboEditor::operator squi::Child() const {
	auto storage = std::make_shared<Storage>(combo);

	VoidObservable closeEvent{};
	VoidObservable nodeListChangedEvent{};

	return Dialog{
		.width = 1000.f,
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
					.buildFunc = std::bind(comboEditorEntries, storage, ctx, characterKey, nodeListChangedEvent),
				},
				Row{
					.children{
						Button{
							.text = "Add node",
							.onClick = [characterKey = characterKey, storage, nodeListChangedEvent, ctx = ctx](GestureDetector::Event event) {
								event.widget.addOverlay(NodePicker{
									.characterKey = characterKey,
									.enableCombos = false,
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
