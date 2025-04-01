#include "optionPicker.hpp"
#include "Ui/utils/masonry.hpp"
#include "align.hpp"
#include "button.hpp"
#include "character/data.hpp"
#include "character/instance.hpp"
#include "container.hpp"
#include "dialog.hpp"
#include "stats/team.hpp"
#include "text.hpp"

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
}// namespace

UI::OptionPicker::operator squi::Child() const {
	VoidObservable closeEvent;

	return Dialog{
		.width = 800.f,
		.closeEvent = closeEvent,
		.title = "Choose node",
		.content = Masonry{
			.spacing = 4.f,
			.columnCount = Masonry::MinSize{200.f},
			.children = [&]() {
				Children ret;

				for (const auto &character: ctx.team.characters) {
					if (!character) continue;

					for (const auto &optListPtr: Option::CharacterList::getMembers()) {
						auto &optList = std::invoke(optListPtr, character->loadout.character.data.data.opts);
						for (const auto &opt: optList) {
							ret.emplace_back(OptionPickerEntry{
								.name = "asd",
								.option = std::visit(Utils::overloaded{
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
													 opt),
								.onSelect = onSelect,
								.closeEvent = closeEvent,
							});
						}
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
