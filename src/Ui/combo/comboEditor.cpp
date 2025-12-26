#include "comboEditor.hpp"

#include "Ui/combo/nodePicker.hpp"
#include "Ui/option/toggleOption.hpp"
#include "Ui/option/valueListOption.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/masonry.hpp"
#include "Ui/utils/tag.hpp"
#include "character/data.hpp"
#include "formula/combo.hpp"
#include "formula/custom.hpp"
#include "optionPicker.hpp"
#include "reaction/list.hpp"
#include "store.hpp"
#include "utils/overloaded.hpp"
#include "weapon/data.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/dialog.hpp"
#include "widgets/dropdownButton.hpp"
#include "widgets/expander.hpp"
#include "widgets/iconButton.hpp"
#include "widgets/navigator.hpp"
#include "widgets/numberBox.hpp"
#include "widgets/row.hpp"
#include "widgets/text.hpp"
#include "widgets/textBox.hpp"


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
				[](const Combo::Source::Weapon &) -> std::string {
					return "Weapon";
				},
				[](const Combo::Source::Artifact &) -> std::string {
					return "Artifact";
				},
				[](const Combo::Source::Combo &) -> std::string {
					return "Combo";
				},
				[](const Combo::Source::TransformativeReaction &) -> std::string {
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
		Args widget{};
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
							.crossAxisAlignment = Row::Alignment::center,
							.spacing = 4.f,
							.children{
								Text{
									.text = std::string(::Store::characters.at(option.key).state.stats.data.name),
									.lineWrap = true,
								},
								UI::Tag{.sourceStr = getOptionSourceName(option.key, option.hash)},
								Container{},
								IconButton{
									.icon = 0xe5cd,
									.theme = Button::Theme::Subtle(),
									.onClick = [&entry = entry, hash = option.hash]() {
										std::visit(
											[&hash](auto &&entry) {
												entry.options.erase(
													std::remove_if(entry.options.begin(), entry.options.end(), [&](const Combo::Option &val) {
														return val.hash == hash;
													}),
													entry.options.end()
												);
												entry.optionUpdateEvent.notify();
											},
											entry
										);
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

	Child createComboOptionEntries(Combo::EntryTypes &entry, Formula::Context ctx) {
		Children ret = [&entry, ctx]() mutable {
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
					.compileFunc = [](const Formula::Context &) {
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
													.option = optRef,
													.instanceKey = character->instanceKey,
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
													.option = optRef,
													.instanceKey = character->instanceKey,
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
			},
			.columnCount = UI::Masonry::MinSize{250.f},
			.spacing = 4.f,
			.children = ret,
		};
	}

}// namespace

squi::core::Child UI::ComboEditor::State::build(const Element &element) {
	return Dialog{
		.width = 1000.f,
		.closeEvent = closeEvent,
		.title = "Edit combo",
		.content = Column{
			.spacing = 4.f,
			.children{
				Expander{
					.title = "Combo name",
					.action = Row{
						.crossAxisAlignment = Row::Alignment::center,
						.children{
							Button{
								.onClick = [&]() {
									Navigator::of(element).pushOverlay(NodePicker{
										.characterKey = widget->characterKey,
										.enableCombos = false,
										.ctx = widget->ctx,
										.onSelect = [this](Combo::Source::Types source) {
											setState([&]() {
												combo.entries.emplace_back(Combo::Entry{
													.multiplier = 1.f,
													.source = source,
												});
											});
										},
									});
								},
								.child = "Add node",
							},
							Button{
								.onClick = [this]() {
									setState([&]() {
										combo.entries.emplace_back(Combo::StateChangeEntry{});
									});
								},
								.child = "Add override",
							},
							TextBox{
								.controller = nameController,
							},
						},
					},
				},
				Column{
					.spacing = 4.f,
					.children = [&]() {
						Children ret;

						std::vector<::Combo::Option> optionStack;

						for (auto it = combo.entries.begin(); it != combo.entries.end(); it++) {
							auto &entry = *it;
							Child multiplierBox;
							std::string captionText;
							std::string reactionName;
							Child reactionSelector;
							size_t reactionSelectorSize = 0;

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
										entryColor = Node::getColor(node.data, widget->ctx);

										std::optional<Misc::Element> element = std::nullopt;

										std::visit(
											Utils::overloaded{
												[&](const Node::AtkData &data) {
													element = Formula::getElement(
														data.source,
														data.element,
														widget->ctx
													);
												},
												[&](const Node::CustomAtkData &data) {
													element = data.element;
												},
												[](const Node::InfoData &data) {},
												[](const Node::HealData &data) {},
												[](const Node::ShieldData &data) {},
												[](const Node::ModsData &data) {},
												[&](const Node::DirectLunarData &data) {
													element = Misc::lunarDamageTypeToElement(data.damageType);
												},
											},
											node.data
										);

										reactionName = getReactionName(Reaction::List::fromNodeReaction(entry.reaction));

										reactionSelector = DropdownButton{
											.theme = entry.reaction == Misc::NodeReaction::none ? Button::Theme::Standard() : Button::Theme::Accent(),
											.text = reactionName,
											.items = [&]() {
												std::vector<ContextMenu::Item> ret;

												ret.emplace_back(ContextMenu::Button{
													.text = "No reaction",
													.callback = [this, &entry]() {
														setState([&]() {
															entry.reaction = Misc::NodeReaction::none;
														});
													},
												});

												for (const auto &reaction: ::Reaction::List::amplifyingList) {
													if (!element || reaction->trigger != element.value()) continue;
													ret.emplace_back(ContextMenu::Button{
														.text = std::string{reaction->name},
														.callback = [this, &entry, reaction]() {
															setState([&]() {
																entry.reaction = reaction->nodeReaction;
															});
														},
													});
												}
												for (const auto &reaction: ::Reaction::List::additiveList) {
													if (!element || reaction->trigger != element.value()) continue;
													ret.emplace_back(ContextMenu::Button{
														.text = std::string{reaction->name},
														.callback = [this, &entry, reaction]() {
															setState([&]() {
																entry.reaction = reaction->nodeReaction;
															});
														},
													});
												}

												reactionSelectorSize = ret.size();
												return ret;
											}(),
										};
										multiplierBox = NumberBox{
											.widget{
												.width = 48.f,
											},
											.value = entry.multiplier,
											.onChange = [this, &entry](float newVal) {
												setState([&]() {
													entry.multiplier = newVal;
												});
											},
										};
										captionText = [this, &entry, optionStack]() {
											std::vector<::Combo::Option> store;
											auto newCtx = widget->ctx.withReaction(Reaction::List::fromNodeReaction(entry.reaction));
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
										}();
									},
									[&](const ::Combo::StateChangeEntry &entry) {
										entryName = "Option override";
										optionStack.insert(optionStack.end(), entry.options.begin(), entry.options.end());
									},
								},
								entry
							);

							auto addOptionOverride = Button{
								.theme = Button::Theme::Standard(),
								.onClick = [this, &entry]() {
									auto [options, optionUpdateEvent] = std::visit(
										[](auto &&entry) -> auto {
											return std::make_pair(std::ref(entry.options), std::ref(entry.optionUpdateEvent));
										},
										entry
									);
									Navigator::of(*this->element).pushOverlay(UI::OptionPicker{
										.characterKey = widget->characterKey,
										.ctx = widget->ctx,
										.options = options,
										.onSelect = [this, &options](Combo::Option option) {
											setState([&]() {
												options.emplace_back(option);
											});
										},
									});
								},
								.child = "Add option",
							};

							auto deleteButton = IconButton{
								.icon = 0xe5cd,
								.theme = Button::Theme::Subtle(),
								.onClick = [this, entryPtr = &entry]() {
									setState([&]() {
										combo.entries.remove_if([entryPtr](const Combo::EntryTypes &entry) {
											return &entry == entryPtr;
										});
									});
								},
							};

							auto moveUpButton = IconButton{
								.widget{
									.width = Size::Shrink,
									.padding = Padding(8.f, 6.f),
								},
								.icon = 0xe316,
								.theme = Button::Theme::Standard(),
								.disabled = &entry == &combo.entries.front(),
								.onClick = [this, it, &entry]() {
									setState([&]() {
										std::swap(entry, *std::next(it, -1));
									});
								},
							};
							auto moveDownButton = IconButton{
								.widget{
									.width = Size::Shrink,
									.padding = Padding(8.f, 6.f),
								},
								.icon = 0xe313,
								.theme = Button::Theme::Standard(),
								.disabled = &entry == &combo.entries.back(),
								.onClick = [this, it, &entry]() {
									setState([&]() {
										std::swap(entry, *std::next(it, 1));
									});
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
								.title = Row{
									.crossAxisAlignment = Row::Alignment::center,
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
								.subtitle = captionText,
								.alwaysExpanded = true,
								.action = Row{
									.children{
										reactionSelectorSize <= 1 ? Child{} : reactionSelector,
										addOptionOverride,
										deleteButton,
									},
								},
								.content = createComboOptionEntries(std::ref(entry), widget->ctx),
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
				.theme = Button::Theme::Accent(),
				.onClick = [this]() {
					if (widget->onSave) widget->onSave(combo);
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
		},
	};
}
