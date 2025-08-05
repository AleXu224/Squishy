#include "comboEditor.hpp"

#include "Ui/combo/nodePicker.hpp"
#include "Ui/option/toggleOption.hpp"
#include "Ui/option/valueListOption.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/masonry.hpp"
#include "Ui/utils/tag.hpp"
#include "button.hpp"
#include "character/data.hpp"
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
#include "weapon/data.hpp"
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
	[[nodiscard]] std::string getOptionSourceName(Character::InstanceKey key, uint32_t hash) {
		const auto &character = Store::characters.at(key);
		const auto &charOpts = character.state.stats.data.data.opts;
		for (const auto &slot: Node::characterSlots) {
			const auto &optList = charOpts.fromCharacterSlot(slot);
			for (const auto &opt: optList) {
				auto key = std::visit(
					[](auto &&val) {
						return val.key;
					},
					opt
				);
				if (key.hash == hash) return Utils::Stringify(slot);
			}
		}
		const auto &weaponOpts = character.state.loadout().weapon->data->data.opts;
		for (const auto &opt: weaponOpts) {
			auto key = std::visit(
				[](auto &&val) {
					return val.key;
				},
				opt
			);
			if (key.hash == hash) return Utils::Stringify(character.state.loadout().weapon->data->name);
		}
		const auto &artifactOpts1 = character.state.loadout().artifact.bonus1;
		const auto &artifactOpts2 = character.state.loadout().artifact.bonus2;
		for (const auto &bonus: {artifactOpts1, artifactOpts2}) {
			if (!bonus) continue;
			for (const auto &opt: bonus->bonusPtr->opts) {
				auto key = std::visit(
					[](auto &&val) {
						return val.key;
					},
					opt
				);
				if (key.hash == hash) return Utils::Stringify(character.state.loadout().weapon->data->name);
			}
		}

		return "Unknown";
	}

	struct ComboEditorOptionContainer {
		// Args
		squi::Widget::Args widget{};
		::Combo::EntryTypes &entry;
		::Combo::Option option;
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
									.text = ::Store::characters.at(option.key).state.stats.data.name,
									.lineWrap = true,
								},
								UI::Tag{.sourceStr = getOptionSourceName(option.key, option.hash)},
								Container{},
								IconButton{
									.icon = 0xe5cd,
									.style = ButtonStyle::Subtle(),
									.onClick = [&entry = entry, hash = option.hash](GestureDetector::Event event) {
										std::visit([&hash](auto &&entry) {
											entry.options.erase(
												std::remove_if(entry.options.begin(), entry.options.end(), [&](const Combo::Option &val) {
													return val.hash == hash;
												}),
												entry.options.end()
											);
											entry.optionUpdateEvent.notify();
										},
												   entry);
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

	Child createComboOptionEntries(Combo::EntryTypes &entry, std::shared_ptr<UI::ComboEditor::Storage> storage, Formula::Context ctx, VoidObservable nodeListChangedEvent, Theme theme) {
		auto _ = ThemeManager::pushTheme(theme);
		Children ret = [&entry, storage, ctx, nodeListChangedEvent]() mutable {
			std::vector<Combo::Option> store;
			ctx.optionStore = &store;

			Children ret;
			auto [options, optionUpdateEvent] = std::visit([](auto &&entry) {
				return std::make_pair(std::ref(entry.options), std::ref(entry.optionUpdateEvent));
			},
														   entry);

			auto node = Formula::ComboOptionOverride{
				.overrides = options,
				.node = Formula::Custom{
					.compileFunc = [](const Formula::Context &ctx) {
						return Formula::Compiled::ConstantFloat{.value = 0.f}.wrap();
					},
					.func = [&](const Formula::Context &ctx) {
						for (auto &opt: options) {
							for (const auto &character: ctx.team.characters) {
								if (!character) continue;
								if (character->instanceKey != opt.key) continue;
								if (!character->state.options.contains(opt.hash)) continue;

								auto optPtr = std::make_shared<Option::Types>(character->state.options.at(opt.hash));
								std::visit(
									Utils::overloaded{
										[&](bool value) {
											auto &optRef = std::get<Option::Boolean>(*optPtr);
											optRef.active = value;
											squi::Observable<bool> switchEvent{};
											ret.emplace_back(ComboEditorOptionContainer{
												.entry = entry,
												.option = opt,
												.child = UI::ToggleOption{
													.widget{
														.onInit = [switchEvent, &opt, &entry, storage, nodeListChangedEvent](Widget &w) {
															observe(w, switchEvent, [&opt, &entry, storage, nodeListChangedEvent](bool value) {
																opt.value = value;
																std::visit(
																	Utils::overloaded{
																		[](const ::Combo::Entry &entry) {
																			entry.optionUpdateEvent.notify();
																		},
																		[nodeListChangedEvent](const ::Combo::StateChangeEntry &entry) {
																			nodeListChangedEvent.notify();
																		},
																	},
																	entry
																);
															});
														},
														.onUpdate = [optPtr](Widget &) {},
													},
													.option = optRef,
													.instanceKey = character->instanceKey,
													.switchEvent = switchEvent,
													.ctx = ctx.withSource(character->state),
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
												.child = UI::ValueListOption{
													.widget{
														.onInit = [valueChangedEvent, &opt, &entry, storage, nodeListChangedEvent](Widget &w) {
															observe(w, valueChangedEvent, [&opt, &entry, storage, nodeListChangedEvent](std::optional<uint32_t>, std::optional<uint32_t> index) {
																opt.value = index;
																std::visit(
																	Utils::overloaded{
																		[](const ::Combo::Entry &entry) {
																			entry.optionUpdateEvent.notify();
																		},
																		[nodeListChangedEvent](const ::Combo::StateChangeEntry &entry) {
																			nodeListChangedEvent.notify();
																		},
																	},
																	entry
																);
															});
														},
														.onUpdate = [optPtr](Widget &) {},
													},
													.option = optRef,
													.instanceKey = character->instanceKey,
													.valueChangedEvent = valueChangedEvent,
													.ctx = ctx.withSource(character->state),
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

	Child comboEditorEntries(std::shared_ptr<UI::ComboEditor::Storage> storage, Formula::Context ctx, Character::InstanceKey characterKey, VoidObservable nodeListChangedEvent, Theme theme) {
		auto _ = ThemeManager::pushTheme(theme);
		VoidObservable dmgValueUpdateEvent;
		Children ret;

		std::vector<::Combo::Option> optionStack;

		for (auto it = storage->combo.entries.begin(); it != storage->combo.entries.end(); it++) {
			auto &entry = *it;
			Child multiplierBox;
			Child captionText;
			std::string reactionName;
			DropdownButton reactionSelector;

			std::string entryName;
			std::string sourceName;
			Color entryColor = Color::white;
			std::visit(
				Utils::overloaded{
					[&](::Combo::Entry &entry) {
						const auto &node = std::visit(
							[&](auto &&val) {
								return val.resolve(entry.options);
							},
							entry.source
						);

						entryName = node.name;
						sourceName = getSourceName(entry.source);
						entryColor = Node::getColor(node.data, ctx);

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
								[&](const Node::CustomAtkData &data) {
									element = data.element;
								},
								[](const Node::InfoData &data) {},
								[](const Node::HealData &data) {},
								[](const Node::ShieldData &data) {},
								[](const Node::ModsData &data) {},
							},
							node.data
						);

						decltype(DropdownButton::textUpdater) textUpdater{};

						reactionName = getReactionName(Reaction::List::fromNodeReaction(entry.reaction));

						Observable<ButtonStyle> reactionSelectorStyleEvent;
						reactionSelector = DropdownButton{
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
										.content = [&entry, reaction, textUpdater, reactionSelectorStyleEvent, dmgValueUpdateEvent, theme]() {
											entry.reaction = reaction->nodeReaction;
											auto _ = ThemeManager::pushTheme(theme);
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
										.content = [&entry, reaction, textUpdater, reactionSelectorStyleEvent, dmgValueUpdateEvent, theme]() {
											entry.reaction = reaction->nodeReaction;
											auto _ = ThemeManager::pushTheme(theme);
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
						multiplierBox = NumberBox{
							.widget{
								.width = 48.f,
							},
							.value = entry.multiplier,
							.onChange = [&entry](float newVal) {
								entry.multiplier = newVal;
							},
						};
						auto makeCaptionStr = [&entry, ctx, optionStack]() {
							std::vector<::Combo::Option> store;
							auto newCtx = ctx.withReaction(Reaction::List::fromNodeReaction(entry.reaction));
							const auto &node = std::visit(
								[&](auto &&val) {
									auto options = optionStack;
									options.insert(options.end(), entry.options.begin(), entry.options.end());
									return val.resolve(options);
								},
								entry.source
							);
							newCtx.optionStore = &store;
							return std::format("{:.1f}", node.formula.eval(newCtx));
						};
						auto captionStr = makeCaptionStr();

						captionText = Text{
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
					},
					[&](const ::Combo::StateChangeEntry &entry) {
						entryName = "Option override";
						optionStack.insert(optionStack.end(), entry.options.begin(), entry.options.end());
					},
				},
				entry
			);

			auto addOptionOverride = Button{
				.text = "Add option",
				.style = ButtonStyle::Standard(),
				.onClick = [characterKey = characterKey, ctx = ctx, &entry, theme](GestureDetector::Event event) {
					auto _ = ThemeManager::pushTheme(theme);
					auto [options, optionUpdateEvent] = std::visit(
						[](auto &&entry) -> auto {
							return std::make_pair(std::ref(entry.options), std::ref(entry.optionUpdateEvent));
						},
						entry
					);
					event.widget.addOverlay(UI::OptionPicker{
						.characterKey = characterKey,
						.ctx = ctx,
						.options = options,
						.onSelect = [&options, &optionUpdateEvent](Combo::Option option) {
							options.emplace_back(option);
							optionUpdateEvent.notify();
						},
					});
				},
			};

			auto deleteButton = IconButton{
				.icon = 0xe5cd,
				.style = ButtonStyle::Subtle(),
				.onClick = [storage, entryPtr = &entry, nodeListChangedEvent](GestureDetector::Event event) {
					storage->combo.entries.remove_if([entryPtr, nodeListChangedEvent](const Combo::EntryTypes &entry) {
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
							.text = entryName,
							.lineWrap = true,
							.color = entryColor,
						},
						sourceName.empty() ? Child{} : UI::Tag{.sourceStr = sourceName},
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
					.rebuildEvent = std::visit(//
						[](auto &&entry) {
							return entry.optionUpdateEvent;
						},
						entry
					),
					.buildFunc = std::bind(createComboOptionEntries, std::ref(entry), storage, ctx, nodeListChangedEvent, theme),
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
							.onClick = [characterKey = characterKey, storage, nodeListChangedEvent, ctx = ctx, theme = ThemeManager::getTheme()](GestureDetector::Event event) {
								auto _ = ThemeManager::pushTheme(theme);
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
						Button{
							.text = "Add override",
							.onClick = [storage, nodeListChangedEvent](GestureDetector::Event event) {
								storage->combo.entries.emplace_back(Combo::StateChangeEntry{});
								nodeListChangedEvent.notify();
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
					.buildFunc = std::bind(comboEditorEntries, storage, ctx, characterKey, nodeListChangedEvent, ThemeManager::getTheme()),
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
