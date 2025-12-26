#include "characterLoadouts.hpp"

#include "Ui/loadout/loadoutCard.hpp"
#include "observer.hpp"
#include "ranges"
#include "store.hpp"
#include "weapon/defaultWeapons.hpp"
#include "widgets/column.hpp"
#include "widgets/dialog.hpp"
#include "widgets/dropdownButton.hpp"
#include "widgets/expander.hpp"


using namespace squi;

namespace {
	struct CharacterLoadoutsHeadingEntry : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey;

		[[nodiscard]] Child build(const Element &) const {
			return Expander{
				.title = "Add loadout",
				.alwaysExpanded = true,
				.action{
					DropdownButton{
						.text = "Add",
						.items{
							ContextMenu::Button{
								.text = "Build",
								.callback = [characterKey = characterKey]() {
									auto &character = ::Store::characters.at(characterKey);
									auto &weapon = Store::createWeapon(Weapon::defaultWeapons.at(character.state.stats.base.weaponType));
									character.state.loadouts.emplace_back(Stats::Loadout{
										.weaponInstanceKey = weapon.instanceKey,
										.weapon = &weapon.stats,
										.artifact{},
									});
									character.updateEvent.notify();
								},
							},
							ContextMenu::Button{
								.text = "TC Build",
								.callback = [characterKey = characterKey]() {
									auto &character = ::Store::characters.at(characterKey);
									auto &weapon = Store::createWeapon(Weapon::defaultWeapons.at(character.state.stats.base.weaponType));
									character.state.loadouts.emplace_back(Stats::Loadout{
										.weaponInstanceKey = weapon.instanceKey,
										.weapon = &weapon.stats,
										.artifact{
											.equipped = Stats::Artifact::Theorycraft{},
										},
									});
									character.updateEvent.notify();
								},
							},
						},
					},
				},
			};
		}
	};
}// namespace

squi::core::Child UI::CharacterLoadouts::State::build(const Element &element) {
	auto &character = Store::characters.at(widget->characterKey);

	Child content = Column{
		.spacing = 8.f,
		.children = [&]() {
			Children ret{
				CharacterLoadoutsHeadingEntry{
					.characterKey = widget->characterKey,
				},
				LoadoutCard{
					.characterKey = widget->characterKey,
				},
			};

			for (const auto &[index, loadout]: std::views::enumerate(character.state.loadouts)) {
				ret.emplace_back(LoadoutCard{.characterKey = widget->characterKey, .loadoutIndex = index});
			}

			return ret;
		}(),
	};

	Children buttonFooter{
		Button{
			.widget{
				.alignment = Alignment::CenterRight,
			},
			.theme = Button::Theme::Standard(),
			.onClick = [this]() {
				closeEvent.notify();
			},
			.child = "Close",
		},
	};


	return Dialog{
		.width = 1280.f,
		.closeEvent = closeEvent,
		.title = "Loadouts",
		.content = content,
		.buttons = buttonFooter,
	};
}
