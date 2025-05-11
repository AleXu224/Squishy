#include "characterLoadouts.hpp"

#include "Ui/loadout/loadoutCard.hpp"
#include "button.hpp"
#include "column.hpp"
#include "container.hpp"
#include "dialog.hpp"
#include "dropdownButton.hpp"
#include "expander.hpp"
#include "gestureDetector.hpp"
#include "observer.hpp"
#include "ranges"
#include "rebuilder.hpp"
#include "store.hpp"
#include "theme.hpp"
#include "weapon/defaultWeapons.hpp"


using namespace squi;

namespace {
	struct CharacterLoadoutsHeadingEntry {
		// Args
		squi::Widget::Args widget{};
		Character::InstanceKey characterKey;

		operator squi::Child() const {
			return Expander{
				.heading = "Add loadout",
				.alwaysExpanded = true,
				.actions{
					DropdownButton{
						.text = "Add",
						.items{
							ContextMenu::Item{
								.text = "Build",
								.content = [characterKey = characterKey]() {
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
							ContextMenu::Item{
								.text = "TC Build",
								.content = [characterKey = characterKey]() {
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

UI::CharacterLoadouts::operator Child() const {
	VoidObservable closeEvent{};
	auto &character = Store::characters.at(characterKey);
	auto theme = ThemeManager::getTheme();

	auto content = Rebuilder{
		.rebuildEvent = character.updateEvent,
		.buildFunc = [characterKey = characterKey, theme]() -> Child {
			auto _ = ThemeManager::pushTheme(theme);
			return Column{
				.spacing = 8.f,
				.children = [&]() {
					Children ret{
						CharacterLoadoutsHeadingEntry{
							.characterKey = characterKey,
						},
						LoadoutCard{
							.characterKey = characterKey,
						},
					};

					auto &character = Store::characters.at(characterKey);

					for (const auto &[index, loadout]: std::views::enumerate(character.state.loadouts)) {
						ret.emplace_back(LoadoutCard{.characterKey = characterKey, .loadoutIndex = index});
					}

					return ret;
				}(),
			};
		},
	};

	Children buttonFooter{
		Container{},
		Button{
			.text = "Close",
			.style = ButtonStyle::Standard(),
			.onClick = [closeEvent](GestureDetector::Event) {
				closeEvent.notify();
			},
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