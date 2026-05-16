#include "comboEditor.hpp"

#include "UI/combo/nodePicker.hpp"
#include "UI/option/toggleOption.hpp"
#include "UI/option/valueListOption.hpp"
#include "UI/option/valueSliderOption.hpp"
#include "UI/utils/card.hpp"
#include "UI/utils/masonry.hpp"
#include "UI/utils/tag.hpp"
#include "agent/data.hpp"
#include "engine/data.hpp"
#include "formula/attribute.hpp"
#include "optionPicker.hpp"
#include "store.hpp"
#include "utils/overloaded.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/dialog.hpp"
#include "widgets/expander.hpp"
#include "widgets/iconButton.hpp"
#include "widgets/navigator.hpp"
#include "widgets/numberBox.hpp"
#include "widgets/row.hpp"
#include "widgets/text.hpp"
#include "widgets/textBox.hpp"


using namespace squi;

namespace {
	[[nodiscard]] std::string getSourceName(const Combo::Source::Types &source) {
		return std::visit(
			Utils::overloaded{
				[](const Combo::Source::Agent &source) -> std::string {
					return Utils::Stringify(source.slot);
				},
				[](const Combo::Source::Engine &) -> std::string {
					return "Engine";
				},
				[](const Combo::Source::Disc &) -> std::string {
					return "Disc";
				},
				[](const Combo::Source::Combo &) -> std::string {
					return "Combo";
				},
				[](const Combo::Source::Anomaly &) -> std::string {
					return "Anomaly";
				},
			},
			source
		);
	}
	[[nodiscard]] std::string getOptionSourceName(Agent::InstanceKey key, uint32_t hash) {
		if (!key) {
			return "Resonances";
		}

		const auto &agent = Store::agents.at(key);
		const auto &charOpts = agent.state.stats.data.data->opts;
		for (const auto &slot: Node::agentSlots) {
			const auto &optList = charOpts.fromAgentSlot(slot);
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
		const auto &engineOpts = agent.state.loadout().engine->data->data.opts;
		for (const auto &opt: engineOpts) {
			auto key = std::visit(
				[](auto &&val) {
					return val.key;
				},
				opt
			);
			if (key.hash == hash) return agent.state.loadout().engine->data->name;
		}
		const auto &discOpts1 = agent.state.loadout().disc.bonus1;
		const auto &discOpts2 = agent.state.loadout().disc.bonus2;
		const auto &discOpts3 = agent.state.loadout().disc.bonus3;
		for (const auto &bonus: {discOpts1, discOpts2, discOpts3}) {
			if (!bonus) continue;
			for (const auto &opt: bonus->bonusPtr->opts) {
				auto key = std::visit(
					[](auto &&val) {
						return val.key;
					},
					opt
				);
				if (key.hash == hash) return std::string{bonus->setPtr->name};
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
									.text = option.key ? std::string(::Store::agents.at(option.key).state.stats.data.name) : "Team",
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
			Combo::Overrides overrides;

			void init() {
				optionUpdateEvent = std::visit(
					[&](auto &&entry) {
						return entry.optionUpdateEvent.observe([this]() {
							setState([&]() {});
							if (std::holds_alternative<Combo::StateChangeEntry>(widget->entry)) {
								widget->comboUpdateEvent.notify();
							}
						});
					},
					widget->entry
				);
				overrides = widget->overrides;
			}

			void initState() override {
				init();
			}

			void widgetUpdated() override {
				init();
			}

			Child build(const Element &element) override {
				auto ctx = widget->ctx.withOverrides(&overrides);
				std::visit(
					[&](auto &&val) {
						overrides.push(val.options);
					},
					widget->entry
				);

				Children ret;
				auto [options, optionUpdateEvent] = std::visit(
					[](auto &&entry) {
						return std::make_pair(std::ref(entry.options), std::ref(entry.optionUpdateEvent));
					},
					widget->entry
				);

				for (auto &opt: options) {
					const Option::TypesMap &optsMap = [&]() -> const Option::TypesMap & {
						if (opt.key) {
							return Store::agents.at(opt.key).state.options;
						}
						return ctx.team.options;
					}();
					std::variant<Agent::InstanceKey, Team::InstanceKey> sourceKey = opt.key ? std::variant<Agent::InstanceKey, Team::InstanceKey>{opt.key}
																							: ctx.team.instanceKey;
					// auto &agent = Store::agents.at(opt.key);
					if (!optsMap.contains(opt.hash)) continue;
					if (opt.key && !ctx.team.hasAgent(opt.key.key)) continue;

					auto usedCtx = opt.key ? ctx.withSource(Store::agents.at(opt.key).state) : ctx;

					std::visit(
						Utils::overloaded{
							[&](bool &value) {
								localOptions.insert_or_assign(opt.hash, std::get<Option::Boolean>(optsMap.at(opt.hash)));
								auto &optRef = std::get<Option::Boolean>(localOptions.at(opt.hash));
								optRef.active = value;

								ret.emplace_back(ComboEditorOptionContainer{
									.entry = widget->entry,
									.option = opt,
									.child = UI::ToggleOption{
										.option = optRef,
										.instanceKey = sourceKey,
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
										.ctx = usedCtx,
									},
								});
							},
							[&](std::optional<uint8_t> &value) {
								localOptions.insert_or_assign(opt.hash, std::get<Option::ValueList>(optsMap.at(opt.hash)));
								auto &optRef = std::get<Option::ValueList>(localOptions.at(opt.hash));
								optRef.currentIndex = value;
								ret.emplace_back(ComboEditorOptionContainer{
									.entry = widget->entry,
									.option = opt,
									.child = UI::ValueListOption{
										.option = optRef,
										.instanceKey = sourceKey,
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
										.ctx = usedCtx,
									},
								});
							},
							[&](::Combo::ComboFloatOption &value) {
								localOptions.insert_or_assign(opt.hash, std::get<Option::ValueSlider>(optsMap.at(opt.hash)));
								auto &optRef = std::get<Option::ValueSlider>(localOptions.at(opt.hash));
								optRef.value = value.value;
								ret.emplace_back(ComboEditorOptionContainer{
									.entry = widget->entry,
									.option = opt,
									.child = UI::ValueSliderOption{
										.option = optRef,
										.instanceKey = sourceKey,
										.onChange = [&](float newVal) {
											std::visit(
												[&](auto &&entry) {
													value.value = newVal;
													entry.optionUpdateEvent.notify();
													widget->comboUpdateEvent.notify();
												},
												widget->entry
											);
										},
										.ctx = usedCtx,
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
										.agentKey = widget->agentKey,
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

						overrides = Combo::Overrides{};
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

										entryName = Node::getName(node.data, ctxWithOverrides);
										sourceName = getSourceName(entry.source);
										entryColor = Node::getColor(node.data, ctxWithOverrides);

										std::optional<Misc::Attribute> attribute = std::nullopt;

										std::visit(
											Utils::overloaded{
												[&](const Node::AtkData &data) {
													attribute = Formula::getAttribute(
														data.source,
														data.attribute,
														ctxWithOverrides
													);
												},
												[&](const Node::DazeData &data) {
													attribute = Formula::getAttribute(
														data.source,
														data.attribute,
														ctxWithOverrides
													);
												},
												[&](const Node::CustomAtkData &data) {
													attribute = data.attribute;
												},
												[&](const Node::AbloomData &data) {
													attribute = data.attribute;
												},
												[](const Node::InfoData &data) {},
												[](const Node::HealData &data) {},
												[](const Node::ModsData &data) {},
											},
											node.data
										);

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
										captionText = [ctx = ctxWithOverrides, &entry, &overrides = this->overrides]() mutable {
											const auto &node = std::visit(
												[&](auto &&val) {
													overrides.push(entry.options);
													return val.resolve(overrides);
												},
												entry.source
											);
											return Utils::formatFloat(node.formula.eval(ctx.withOverrides(&overrides)));
										}();
									},
									[&](const ::Combo::StateChangeEntry &entry) {
										entryName = "Option override";
										overrides.push(entry.options);
									},
								},
								entry
							);

							Child addOptionOverride = Button{
								.theme = Button::Theme::Standard(),
								.onClick = [this, &entry, ctxWithOverrides]() {
									auto [options, optionUpdateEvent] = std::visit(
										[](auto &&entry) -> auto {
											return std::make_pair(std::ref(entry.options), std::ref(entry.optionUpdateEvent));
										},
										entry
									);
									Navigator::of(*this->element).pushOverlay(UI::OptionPicker{
										.agentKey = widget->agentKey,
										.ctx = ctxWithOverrides,
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

							Child deleteButton = IconButton{
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

							Child moveUpButton = IconButton{
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
							Child moveDownButton = IconButton{
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
									.ctx = ctxWithOverrides,
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
				.theme = Button::Theme::Accent(element),
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
