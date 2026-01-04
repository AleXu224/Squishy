#include "comboEditor.hpp"

#include "Ui/combo/nodePicker.hpp"
#include "Ui/option/toggleOption.hpp"
#include "Ui/option/valueListOption.hpp"
#include "Ui/utils/card.hpp"
#include "Ui/utils/masonry.hpp"
#include "Ui/utils/tag.hpp"
#include "character/data.hpp"
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

	struct ComboOptionEntry : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Combo::EntryTypes &entry;
		VoidObservable comboUpdateEvent;
		Combo::Overrides overrides;
		Formula::Context ctx;

		struct State : WidgetState<ComboOptionEntry> {
			Option::TypesMap localOptions;
			VoidObserver optionUpdateEvent;

			void initState() override {
				optionUpdateEvent = std::visit(
					[&](auto &&entry) {
						return entry.optionUpdateEvent.observe([this]() {
							setState([&]() {});
						});
					},
					widget->entry
				);
			}

			Child build(const Element &element) override {
				auto ctx = widget->ctx.withOverrides(&widget->overrides);

				Children ret;
				auto [options, optionUpdateEvent] = std::visit(
					[](auto &&entry) {
						return std::make_pair(std::ref(entry.options), std::ref(entry.optionUpdateEvent));
					},
					widget->entry
				);

				for (auto &opt: options) {
					auto &character = Store::characters.at(opt.key);
					if (!character.state.options.contains(opt.hash)) continue;
					if (!ctx.team.hasCharacter(opt.key.key)) continue;

					std::visit(
						Utils::overloaded{
							[&](bool &value) {
								localOptions.insert_or_assign(opt.hash, std::get<Option::Boolean>(character.state.options.at(opt.hash)));
								auto &optRef = std::get<Option::Boolean>(localOptions.at(opt.hash));
								optRef.active = value;

								ret.emplace_back(ComboEditorOptionContainer{
									.entry = widget->entry,
									.option = opt,
									.child = UI::ToggleOption{
										.option = optRef,
										.instanceKey = character.instanceKey,
										.onToggle = [&](bool newVal) {
											std::visit(
												[&](auto &&entry) {
													value = newVal;
													entry.optionUpdateEvent.notify();
													widget->comboUpdateEvent.notify();
												},
												widget->entry
											);
										},
										.ctx = ctx.withSource(character.state),
									},
								});
							},
							[&](std::optional<uint8_t> &value) {
								localOptions.insert_or_assign(opt.hash, std::get<Option::ValueList>(character.state.options.at(opt.hash)));
								auto &optRef = std::get<Option::ValueList>(localOptions.at(opt.hash));
								optRef.currentIndex = value;
								ret.emplace_back(ComboEditorOptionContainer{
									.entry = widget->entry,
									.option = opt,
									.child = UI::ValueListOption{
										.option = optRef,
										.instanceKey = character.instanceKey,
										.onChange = [&](std::optional<uint8_t> newVal) {
											std::visit(
												[&](auto &&entry) {
													value = newVal;
													entry.optionUpdateEvent.notify();
													widget->comboUpdateEvent.notify();
												},
												widget->entry
											);
										},
										.ctx = ctx.withSource(character.state),
									},
								});
							},
						},
						opt.value
					);
				}

				if (ret.empty()) return Container{};

				return UI::Masonry{
					.widget{
						.padding = 4.f,
					},
					.columnCount = UI::Masonry::MinSize{250.f},
					.spacing = 4.f,
					.children = ret,
				};
			}
		};
	};
}// namespace

squi::core::Child UI::ComboEditor::State::build(const Element &element) {
	auto ctx = widget->ctx;

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
						.widget{
							.width = Size::Shrink,
						},
						.crossAxisAlignment = Row::Alignment::center,
						.spacing = 4.f,
						.children{
							Button{
								.onClick = [this, ctx]() {
									Navigator::of(this).pushOverlay(NodePicker{
										.characterKey = widget->characterKey,
										.enableCombos = false,
										.ctx = ctx,
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

						Combo::Overrides overrides;
						auto ctxWithOverrides = ctx.withOverrides(&overrides);

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
												return val.resolve(overrides);
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
										captionText = [ctx = ctxWithOverrides, &entry, overrides]() mutable {
											auto newCtx = ctx.withReaction(Reaction::List::fromNodeReaction(entry.reaction));
											const auto &node = std::visit(
												[&](auto &&val) {
													overrides.push(entry.options);
													return val.resolve(overrides);
												},
												entry.source
											);
											return std::format("{:.1f}", node.formula.eval(newCtx.withOverrides(&overrides)));
										}();
									},
									[&](const ::Combo::StateChangeEntry &entry) {
										entryName = "Option override";
										overrides.push(entry.options);
									},
								},
								entry
							);

							auto addOptionOverride = Button{
								.theme = Button::Theme::Standard(),
								.onClick = [this, &entry, ctx]() {
									auto [options, optionUpdateEvent] = std::visit(
										[](auto &&entry) -> auto {
											return std::make_pair(std::ref(entry.options), std::ref(entry.optionUpdateEvent));
										},
										entry
									);
									Navigator::of(*this->element).pushOverlay(UI::OptionPicker{
										.characterKey = widget->characterKey,
										.ctx = ctx,
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
									.widget{
										.width = Size::Wrap,
									},
									.crossAxisAlignment = Row::Alignment::center,
									.spacing = 4.f,
									.children{
										reactionSelectorSize <= 1 ? Child{} : reactionSelector,
										addOptionOverride,
										deleteButton,
									},
								},
								.content = ComboOptionEntry{
									.entry = entry,
									.comboUpdateEvent = comboUpdateEvent,
									.overrides = overrides,
									.ctx = ctx,
								},
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
