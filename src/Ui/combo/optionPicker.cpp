#include "optionPicker.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/masonry.hpp"
#include "character/data.hpp"
#include "character/instance.hpp"
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

				for (const auto &character: widget->ctx.team.characters) {
					if (!character) continue;
					Children characterRet;
					for (const auto &[memberCond, slot]: std::views::zip(Option::CharacterList::getMembersAndConditions(), Node::characterSlots)) {
						auto &[optListPtr, cond] = memberCond;
						auto &optList = std::invoke(optListPtr, character->state.stats.data.data.opts);
						Children categoryRet;
						for (const auto &opt: optList) {
							auto [teamBuff, condition] = std::visit(
								[](auto &&val) {
									return std::tuple{val.teamBuff, val.displayCondition};
								},
								opt
							);
							if (character->instanceKey != widget->characterKey && !teamBuff) continue;
							auto newCtx = widget->ctx.withSource(character->state);
							if (condition.hasValue() && !condition.eval(newCtx)) continue;
							if (!cond.eval(newCtx)) continue;
							if (existingOptions.contains(
									std::visit(
										[](auto &&val) {
											return val.key.hash;
										},
										opt
									)
								)) continue;
							categoryRet.emplace_back(OptionPickerEntry{
								.name = std::visit(//
									Utils::overloaded{
										[&](const Option::Boolean &opt) {
											return std::string{opt.name};
										},
										[&](const Option::ValueList &opt) {
											return std::string{opt.prefix};
										},
									},
									opt
								),
								.option = std::visit(//
									Utils::overloaded{
										[&](const Option::Boolean &opt) {
											return Combo::Option{
												.key = character->instanceKey,
												.hash = opt.key.hash,
												.value = opt.active,
											};
										},
										[&](const Option::ValueList &opt) {
											return Combo::Option{
												.key = character->instanceKey,
												.hash = opt.key.hash,
												.value = opt.currentIndex,
											};
										},
									},
									opt
								),
								.onSelect = widget->onSelect,
								.closeEvent = closeEvent,
							});
						}
						if (!categoryRet.empty()) {
							characterRet.emplace_back(OptionPickerCategory{
								.name = Utils::Stringify(slot),
								.children = categoryRet,
							});
						}
					}
					if (!characterRet.empty()) {
						ret.emplace_back(DisplayCard{
							.title = std::string(character->state.stats.data.name),
							.children = characterRet,
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
