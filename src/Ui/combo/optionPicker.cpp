#include "optionPicker.hpp"
#include "Ui/utils/displayCard.hpp"
#include "Ui/utils/masonry.hpp"
#include "align.hpp"
#include "button.hpp"
#include "character/data.hpp"
#include "character/instance.hpp"
#include "container.hpp"
#include "dialog.hpp"
#include "ranges"
#include "stats/team.hpp"
#include "text.hpp"
#include <unordered_set>


using namespace squi;

namespace {
	struct OptionPickerEntry {
		// Args
		squi::Widget::Args widget{};
		std::string name;
		::Combo::Option option;
		std::function<void(::Combo::Option)> onSelect;
		VoidObservable closeEvent;

		operator squi::Child() const {
			return Button{
				.widget{
					.width = Size::Expand,
					.margin = Margin{4.f, 2.f},
				},
				.style = ButtonStyle::Subtle(),
				.onClick = [onSelect = onSelect, option = option, closeEvent = closeEvent](auto) {
					if (onSelect) onSelect(option);
					closeEvent.notify();
				},
				.child = Align{
					.xAlign = 0.f,
					.child = Text{
						.text = name,
						.lineWrap = true,
					},
				},
			};
		}
	};

	struct OptionPickerCategory {
		// Args
		squi::Widget::Args widget{};
		std::string name;
		Children children;

		operator squi::Child() const {
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
							.child = Align{
								.xAlign = 0.f,
								.child = Text{
									.text = name,
									.font = FontStore::defaultFontBold,
								}
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

UI::OptionPicker::operator squi::Child() const {
	VoidObservable closeEvent;

	return Dialog{
		.width = 800.f,
		.closeEvent = closeEvent,
		.title = "Choose option",
		.content = Masonry{
			.spacing = 4.f,
			.columnCount = Masonry::MinSize{200.f},
			.children = [&]() {
				Children ret;

				std::unordered_set<uint32_t> existingOptions;
				for (const auto &opt: options) {
					existingOptions.insert(opt.hash);
				}

				for (const auto &character: ctx.team.characters) {
					if (!character) continue;
					Children characterRet;
					for (const auto &[memberCond, slot]: std::views::zip(Option::CharacterList::getMembersAndConditions(), Node::characterSlots)) {
						auto &[optListPtr, cond] = memberCond;
						auto &optList = std::invoke(optListPtr, character->loadout.character.data.data->opts);
						Children categoryRet;
						for (const auto &opt: optList) {
							auto [teamBuff, condition] = std::visit(
								[](auto &&val) {
									return std::tuple{val.teamBuff, val.displayCondition};
								},
								opt
							);
							if (character->instanceKey != characterKey && !teamBuff) continue;
							auto newCtx = ctx.withSource(character->loadout);
							if (condition && !condition->eval(newCtx)) continue;
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
								.onSelect = onSelect,
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
							.title = character->loadout.character.data.name,
							.children = characterRet,
						});
					}
				}

				return ret;
			}(),
		},
		.buttons{
			Container{},
			Button{
				.widget{.width = Size::Expand},
				.text = "Close",
				.onClick = [closeEvent](auto) {
					closeEvent.notify();
				},
			},
		},
	};
}
