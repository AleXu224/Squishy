#include "optionPicker.hpp"
#include "UI/utils/displayCard.hpp"
#include "UI/utils/masonry.hpp"
#include "agent/data.hpp"
#include "agent/instance.hpp"
#include "engine/data.hpp"
#include "range/v3/view/concat.hpp"
#include "range/v3/view/single.hpp"
#include "ranges"
#include "stats/team.hpp"
#include "widgets/button.hpp"
#include "widgets/dialog.hpp"
#include "widgets/text.hpp"
#include <unordered_set>


using namespace squi;

namespace {
	struct OptionPickerEntry : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		std::string name;
		::Combo::Option option;
		std::function<void(::Combo::Option)> onSelect;
		VoidObservable closeEvent;

		[[nodiscard]] Child build(const Element &) const {
			return Button{
				.widget{
					.width = Size::Expand,
					.margin = Margin{4.f, 2.f},
				},
				.theme = Button::Theme::Subtle(),
				.onClick = [this]() {
					if (onSelect) onSelect(option);
					closeEvent.notify();
				},
				.child = Text{
					.widget{
						.alignment = Alignment::CenterLeft,
					},
					.text = name,
					.lineWrap = true,
				},
			};
		}
	};

	struct OptionPickerEntryWrapper : StatelessWidget {
		// Args
		Key key;
		Option::Types opt;
		Agent::InstanceKey agentKey;
		std::function<void(::Combo::Option)> onSelect;
		VoidObservable closeEvent;

		[[nodiscard]] Child build(const Element &) const {
			return OptionPickerEntry{
				.name = std::visit(//
					Utils::overloaded{
						[&](const Option::Boolean &opt) {
							return std::string{opt.name};
						},
						[&](const Option::ValueList &opt) {
							return std::string{opt.prefix};
						},
						[&](const Option::ValueSlider &opt) {
							return std::string{opt.name};
						},
					},
					opt
				),
				.option = std::visit(//
					Utils::overloaded{
						[&](const Option::Boolean &opt) {
							return Combo::Option{
								.key = agentKey,
								.hash = opt.key.hash,
								.value = opt.active,
							};
						},
						[&](const Option::ValueList &opt) {
							return Combo::Option{
								.key = agentKey,
								.hash = opt.key.hash,
								.value = opt.currentIndex,
							};
						},
						[&](const Option::ValueSlider &opt) {
							return Combo::Option{
								.key = agentKey,
								.hash = opt.key.hash,
								.value = Combo::ComboFloatOption{opt.getValue()},
							};
						},
					},
					opt
				),
				.onSelect = onSelect,
				.closeEvent = closeEvent,
			};
		}
	};

	struct OptionPickerCategory : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		std::string name;
		Children children;

		[[nodiscard]] Child build(const Element &) const {
			return Box{
				.widget{
					.margin = Margin{4.f, 2.f},
				},
				.color = Color::css(0x0, 0.25f),
				.borderColor = Color::css(0x0, 0.1f),
				.borderWidth = 1.f,
				.borderRadius = 4.f,
				.child = Column{
					.children{
						Box{
							.widget{
								.padding = Padding{20.f, 16.f},
							},
							.color = Color::transparent,
							.borderColor = Color::css(0x0, 0.1f),
							.borderWidth = BorderWidth::Bottom(1.f),
							.child = Text{
								.widget{
									.alignment = Alignment::CenterLeft,
								},
								.text = name,
								.font = FontStore::defaultFontBold,
							},
						},
						Column{
							.widget{
								.padding = 4.f,
							},
							.spacing = 4.f,
							.children = children,
						},
					},
				},
			};
		}
	};
}// namespace

squi::core::Child UI::OptionPicker::State::build(const Element &element) {
	return Dialog{
		.width = 800.f,
		.closeEvent = closeEvent,
		.title = "Choose option",
		.content = Masonry{
			.columnCount = Masonry::MinSize{200.f},
			.spacing = 4.f,
			.children = [&]() {
				Children ret;

				std::unordered_set<uint32_t> existingOptions;
				for (const auto &opt: widget->options) {
					existingOptions.insert(opt.hash);
				}

				for (const auto &agent: widget->ctx.team.agents) {
					if (!agent) continue;
					Children agentRet;

					auto characterOpts = Node::agentSlots
									   | std::views::transform([&](Node::AgentSlot slot) {
											 return &agent->state.stats.data.data->opts.fromAgentSlot(slot);
										 });
					auto engineOpts = agent->state.loadout().engine ? &agent->state.loadout().engine->data->data.opts : nullptr;
					auto discOpts1 = agent->state.loadout().disc.bonus1 ? &agent->state.loadout().disc.bonus1->bonusPtr->opts : nullptr;
					auto discOpts2 = agent->state.loadout().disc.bonus2 ? &agent->state.loadout().disc.bonus2->bonusPtr->opts : nullptr;
					auto discOpts3 = agent->state.loadout().disc.bonus3 ? &agent->state.loadout().disc.bonus3->bonusPtr->opts : nullptr;
					auto allOpts = ranges::views::concat(characterOpts, ranges::views::single(engineOpts), ranges::views::single(discOpts1), ranges::views::single(discOpts2), ranges::views::single(discOpts3));

					auto characterConditions = Node::agentSlots
											 | std::views::transform([](Node::AgentSlot slot) {
												   return Option::AgentList::conditionFromAgentSlot(slot);
											   });
					auto otherConditions = std::views::repeat(Formula::BoolNode(Formula::ConstantBool{.value = true})) | std::views::take(4);
					auto allConditions = ranges::views::concat(characterConditions, otherConditions);

					auto characterNames = Node::agentSlots
										| std::views::transform([](Node::AgentSlot slot) {
											  return Utils::Stringify(slot);
										  });
					auto otherNames = std::array<std::string, 4>{
						agent->state.loadout().engine ? std::string(agent->state.loadout().engine->data->name) : "Engine",
						agent->state.loadout().disc.bonus1 ? std::string{agent->state.loadout().disc.bonus1->setPtr->name} : "Disc bonus 1",
						agent->state.loadout().disc.bonus2 ? std::string{agent->state.loadout().disc.bonus2->setPtr->name} : "Disc bonus 2",
						agent->state.loadout().disc.bonus3 ? std::string{agent->state.loadout().disc.bonus3->setPtr->name} : "Disc bonus 3",
					};
					auto allNames = ranges::views::concat(characterNames, otherNames);

					for (const auto &[optList, cond, name]: std::views::zip(allOpts, allConditions, allNames)) {
						Children categoryRet;
						if (!optList) continue;
						for (const auto &opt: *optList) {
							auto teamBuff = std::visit(
								[](auto &&val) {
									return val.teamBuff;
								},
								opt
							);
							if (agent->instanceKey != widget->agentKey && !teamBuff) continue;
							auto newCtx = widget->ctx.withSource(agent->state);
							// if (condition.hasValue() && !condition.eval(newCtx)) continue;
							if (!cond.eval(newCtx)) continue;
							if (existingOptions.contains(
									std::visit(
										[](auto &&val) {
											return val.key.hash;
										},
										opt
									)
								)) continue;

							categoryRet.emplace_back(OptionPickerEntryWrapper{
								.opt = opt,
								.agentKey = agent->instanceKey,
								.onSelect = widget->onSelect,
								.closeEvent = closeEvent,
							});
						}
						if (!categoryRet.empty()) {
							agentRet.emplace_back(OptionPickerCategory{
								.name = name,
								.children = categoryRet,
							});
						}
					}

					if (!agentRet.empty()) {
						ret.emplace_back(DisplayCard{
							.title = std::string(agent->state.stats.data.name),
							.children = agentRet,
						});
					}
				}

				// Team buffs
				{
					auto &teamOptList = widget->ctx.team.options;
					Children teamCategoryRet;
					for (const auto &[key, opt]: teamOptList) {
						auto [teamBuff, condition] = std::visit(
							[](auto &&val) {
								return std::tuple{val.teamBuff, val.displayCondition};
							},
							opt
						);
						auto newCtx = widget->ctx;
						if (condition.hasValue() && !condition.eval(newCtx)) continue;
						if (existingOptions.contains(
								std::visit(
									[](auto &&val) {
										return val.key.hash;
									},
									opt
								)
							)) continue;
						teamCategoryRet.emplace_back(OptionPickerEntryWrapper{
							.opt = opt,
							.agentKey = {},
							.onSelect = widget->onSelect,
							.closeEvent = closeEvent,
						});
					}
					if (!teamCategoryRet.empty()) {
						ret.emplace_back(DisplayCard{
							.title = "Resonances",
							.children{
								teamCategoryRet,
							},
						});
					}
				}

				return ret;
			}(),
		},
		.buttons{
			Button{
				.widget{.width = Size::Expand},
				.onClick = [this]() {
					closeEvent.notify();
				},
				.child = "Close",
			},
		},
	};
}
