#include "comboEditor.hpp"

#include "Ui/combo/nodePicker.hpp"
#include "Ui/option/toggleOption.hpp"
#include "Ui/option/valueListOption.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/masonry.hpp"
#include "button.hpp"
#include "column.hpp"
#include "container.hpp"
#include "dialog.hpp"
#include "dropdownButton.hpp"
#include "expander.hpp"
#include "formula/combo.hpp"
#include "formula/custom.hpp"
#include "numberBox.hpp"
#include "optionPicker.hpp"
#include "reaction/list.hpp"
#include "rebuilder.hpp"
#include "row.hpp"
#include "store.hpp"
#include "text.hpp"
#include "textBox.hpp"
#include "utils/overloaded.hpp"
#include "widgets/iconButton.hpp"


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

	[[nodiscard]] std::string getSourceName(const Combo::Source::Types &source) {
		return std::visit(
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
				[](const Combo::Source::TransformativeReaction &source) -> std::string {
					return "Transformative reaction";
				},
			},
			source
		);
	}

	struct ComboEditorSourceTag {
		// Args
		std::string sourceStr;

		operator squi::Child() const {
			return Box{
				.widget{
					.width = Size::Shrink,
					.height = Size::Shrink,
					.padding = 4.f,
				},
				.color = Color::css(0x0, 0.2f),
				.borderColor = Color::css(0x0, 0.1f),
				.borderWidth = 1.f,
				.borderRadius = 4.f,
				.borderPosition = Box::BorderPosition::inset,
				.child = Text{
					.text = sourceStr,
					.fontSize = 12.f,
					.color = Color::css(0xffffff, 0.75f),
				},
			};
		}
	};

	struct ComboEditorOptionContainer {
		// Args
		squi::Widget::Args widget{};
		::Combo::Entry &entry;
		::Combo::Option option;
		::Combo::Source::Types source;
		Child child;

		operator squi::Child() const {
			return UI::Card{
				.widget = widget,
				.child = Column{
					.widget{
						.padding = 4.f,
					},
					.spacing = 4.f,
					.children{
						Row{
							.widget{
								.padding = Padding{12.f, 4.f},
							},
							.alignment = Row::Alignment::center,
							.spacing = 4.f,
							.children{
								Text{
									.text = ::Store::characters.at(option.key).loadout.character.data.name,
									.lineWrap = true,
								},
								ComboEditorSourceTag{.sourceStr = getSourceName(source)},
								Container{},
								IconButton{
									.icon = 0xe5cd,
									.style = ButtonStyle::Subtle(),
									.onClick = [&entry = entry, hash = option.hash](GestureDetector::Event event) {
										entry.options.erase(
											std::remove_if(entry.options.begin(), entry.options.end(), [&](const Combo::Option &val) {
												return val.hash == hash;
											}),
											entry.options.end()
										);
										entry.optionUpdateEvent.notify();
									},
								},
							},
						},
						child,
					},
				},
			};
		}
	};

	Child createComboOptionEntries(Combo::Entry &entry, std::shared_ptr<UI::ComboEditor::Storage> storage, Formula::Context ctx) {
		Children ret = [&entry, storage, ctx]() mutable {
			std::vector<Combo::Option> store;
			ctx.optionStore = &store;

			Children ret;

			auto node = Formula::ComboOptionOverride{
				.overrides = entry.options,
				.node = Formula::Custom{
					.func = [&](const Formula::Context &ctx) {
						for (auto &opt: entry.options) {
							for (const auto &character: ctx.team.characters) {
								if (!character) continue;
								if (character->instanceKey != opt.key) continue;
								if (!character->loadout.options.contains(opt.hash)) continue;

								auto optPtr = std::make_shared<Option::Types>(character->loadout.options.at(opt.hash));
								std::visit(
									Utils::overloaded{
										[&](bool value) {
											auto &optRef = std::get<Option::Boolean>(*optPtr);
											optRef.active = value;
											squi::Observable<bool> switchEvent{};
											ret.emplace_back(ComboEditorOptionContainer{
												.entry = entry,
												.option = opt,
												.source = entry.source,
												.child = UI::ToggleOption{
													.widget{
														.onInit = [switchEvent, &opt, &entry](Widget &w) {
															observe(w, switchEvent, [&opt, &entry](bool value) {
																opt.value = value;
																entry.optionUpdateEvent.notify();
															});
														},
														.onUpdate = [optPtr](Widget &) {},
													},
													.option = optRef,
													.instanceKey = character->instanceKey,
													.switchEvent = switchEvent,
													.ctx = ctx.withSource(character->loadout),
												},
											});
										},
										[&](std::optional<uint8_t> value) {
											auto &optRef = std::get<Option::ValueList>(*optPtr);
											optRef.currentIndex = value;
											squi::Observable<std::optional<uint32_t>, std::optional<uint32_t>> valueChangedEvent{};
											ret.emplace_back(ComboEditorOptionContainer{
												.entry = entry,
												.option = opt,
												.source = entry.source,
												.child = UI::ValueListOption{
													.widget{
														.onInit = [valueChangedEvent, &opt, &entry](Widget &w) {
															observe(w, valueChangedEvent, [&opt, &entry](std::optional<uint32_t>, std::optional<uint32_t> index) {
																opt.value = index;
																entry.optionUpdateEvent.notify();
															});
														},
														.onUpdate = [optPtr](Widget &) {},
													},
													.option = optRef,
													.instanceKey = character->instanceKey,
													.valueChangedEvent = valueChangedEvent,
													.ctx = ctx.withSource(character->loadout),
												},
											});
										},
									},
									opt.value
								);
							}
						}
						return 0.f;
					},
				},
			};
			(void) node.eval(ctx);

			return ret;
		}();

		if (ret.empty()) return Child{};

		return UI::Masonry{
			.widget{
				.padding = 4.f,
				.afterInit = [](Widget &w) {
					// if (w.getChildren().empty()) w.state.padding = 0.f;
				},
			},
			.spacing = 4.f,
			.columnCount = UI::Masonry::MinSize{250.f},
			.children = ret,
		};
	}

	Child comboEditorEntries(std::shared_ptr<UI::ComboEditor::Storage> storage, Formula::Context ctx, Character::InstanceKey characterKey, VoidObservable nodeListChangedEvent) {
		VoidObservable dmgValueUpdateEvent;
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

			Observable<ButtonStyle> reactionSelectorStyleEvent;
			auto reactionSelector = DropdownButton{
				.widget{
					.onInit = [reactionSelectorStyleEvent](Widget &w) {
						observe(w, reactionSelectorStyleEvent, [&w](ButtonStyle style) {
							Button::State::style.of(w) = style;
						});
					},
				},
				.style = entry.reaction == Misc::NodeReaction::none ? ButtonStyle::Standard() : ButtonStyle::Accent(),
				.text = reactionName,
				.items = [&]() {
					std::vector<ContextMenu::Item> ret;

					ret.emplace_back(ContextMenu::Item{
						.text = "No reaction",
						.content = [&entry, textUpdater, reactionSelectorStyleEvent, dmgValueUpdateEvent]() {
							entry.reaction = Misc::NodeReaction::none;
							reactionSelectorStyleEvent.notify(ButtonStyle::Standard());
							textUpdater.notify("No reaction");
							dmgValueUpdateEvent.notify();
						},
					});

					for (const auto &reaction: ::Reaction::List::amplifyingList) {
						if (!element || reaction->trigger != element.value()) continue;
						ret.emplace_back(ContextMenu::Item{
							.text = std::string{reaction->name},
							.content = [&entry, reaction, textUpdater, reactionSelectorStyleEvent, dmgValueUpdateEvent]() {
								entry.reaction = reaction->nodeReaction;
								reactionSelectorStyleEvent.notify(ButtonStyle::Accent());
								textUpdater.notify(std::string{reaction->name});
								dmgValueUpdateEvent.notify();
							},
						});
					}
					for (const auto &reaction: ::Reaction::List::additiveList) {
						if (!element || reaction->trigger != element.value()) continue;
						ret.emplace_back(ContextMenu::Item{
							.text = std::string{reaction->name},
							.content = [&entry, reaction, textUpdater, reactionSelectorStyleEvent, dmgValueUpdateEvent]() {
								entry.reaction = reaction->nodeReaction;
								reactionSelectorStyleEvent.notify(ButtonStyle::Accent());
								textUpdater.notify(std::string{reaction->name});
								dmgValueUpdateEvent.notify();
							},
						});
					}

					return ret;
				}(),
				.textUpdater = textUpdater,
			};

			auto addOptionOverride = Button{
				.text = "Add option",
				.style = ButtonStyle::Standard(),
				.onClick = [characterKey = characterKey, ctx = ctx, &entry](GestureDetector::Event event) {
					event.widget.addOverlay(UI::OptionPicker{
						.characterKey = characterKey,
						.ctx = ctx,
						.options = entry.options,
						.onSelect = [&entry](Combo::Option option) {
							entry.options.emplace_back(option);
							entry.optionUpdateEvent.notify();
						},
					});
				},
			};

			auto deleteButton = IconButton{
				.icon = 0xe5cd,
				.style = ButtonStyle::Subtle(),
				.onClick = [storage, entryPtr = &entry, nodeListChangedEvent](GestureDetector::Event event) {
					storage->combo.entries.remove_if([entryPtr, nodeListChangedEvent](const Combo::Entry &entry) {
						return &entry == entryPtr;
					});
					nodeListChangedEvent.notify();
				},
			};

			auto moveUpButton = IconButton{
				.widget{
					.width = Size::Shrink,
					.padding = Padding(8.f, 6.f),
				},
				.icon = 0xe316,
				.style = ButtonStyle::Standard(),
				.disabled = &entry == &storage->combo.entries.front(),
				.onClick = [it, &entry, nodeListChangedEvent](GestureDetector::Event event) {
					std::swap(entry, *std::next(it, -1));
					nodeListChangedEvent.notify();
				},
			};
			auto moveDownButton = IconButton{
				.widget{
					.width = Size::Shrink,
					.padding = Padding(8.f, 6.f),
				},
				.icon = 0xe313,
				.style = ButtonStyle::Standard(),
				.disabled = &entry == &storage->combo.entries.back(),
				.onClick = [it, &entry, nodeListChangedEvent](GestureDetector::Event event) {
					std::swap(entry, *std::next(it, 1));
					nodeListChangedEvent.notify();
				},
			};

			auto multiplierBox = NumberBox{
				.widget{
					.width = 48.f,
				},
				.value = entry.multiplier,
				.onChange = [&entry](float newVal) {
					entry.multiplier = newVal;
				},
			};

			auto makeCaptionStr = [&entry, ctx]() {
				std::vector<::Combo::Option> store;
				auto newCtx = ctx.withReaction(Reaction::List::fromNodeReaction(entry.reaction));
				const auto &node = std::visit(
					[&](auto &&val) {
						return val.resolve(entry.options);
					},
					entry.source
				);
				newCtx.optionStore = &store;
				return std::format("{:.1f}", node.formula.eval(newCtx));
			};
			auto captionStr = makeCaptionStr();

			auto captionText = Text{
				.widget{
					.onInit = [dmgValueUpdateEvent, makeCaptionStr, &entry](Widget &w) {
						observe("dmgValueUpdateEvent", w, dmgValueUpdateEvent, [&w, makeCaptionStr]() {
							auto captionStr = makeCaptionStr();

							w.as<Text::Impl>().setText(captionStr);
						});
						observe("optionUpdateevent", w, entry.optionUpdateEvent, [dmgValueUpdateEvent]() {
							dmgValueUpdateEvent.notify();
						});
					},
				},
				.text = captionStr,
				.fontSize = 12.f,
				.lineWrap = true,
				.color = 0xFFFFFFC8,
			};


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
						ComboEditorSourceTag{.sourceStr = getSourceName(entry.source)},
					},
				},
				.caption = captionText,
				.alwaysExpanded = true,
				.actions{
					reactionSelector.items.size() <= 1 ? Child{} : reactionSelector,
					addOptionOverride,
					deleteButton,
				},
				.expandedContent = Rebuilder{
					.rebuildEvent = entry.optionUpdateEvent,
					.buildFunc = std::bind(createComboOptionEntries, std::ref(entry), storage, ctx),
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
				Expander{
					.heading = "Combo name",
					.actions{
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
						TextBox{
							.text = storage->combo.name,
							.controller{
								.onChange = [storage](std::string_view newText) {
									storage->combo.name = newText;
								},
							},
						},
					},
				},
				Rebuilder{
					.rebuildEvent = nodeListChangedEvent,
					.buildFunc = std::bind(comboEditorEntries, storage, ctx, characterKey, nodeListChangedEvent),
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
