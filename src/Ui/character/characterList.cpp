#include "characterList.hpp"

#include "Ui/utils/grid.hpp"
#include "button.hpp"
#include "character/characters.hpp"
#include "characterCard.hpp"
#include "characterDataSelector.hpp"
#include "gestureDetector.hpp"
#include "navigator.hpp"
#include "row.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"
#include "weapon/defaultWeapons.hpp"
#include <GLFW/glfw3.h>


using namespace squi;

namespace {
	[[nodiscard]] auto makeCharacters(const Navigator::Controller &controller) {
		Children ret;
		for (auto &[_, character]: ::Store::characters) {
			ret.emplace_back(UI::CharacterCard{
				.characterKey = character.instanceKey,
				.controller = controller,
			});
		}
		return ret;
	}
}// namespace

UI::CharacterList::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	Navigator::Controller controller{};

	auto characterAddButton = Button{
		.text = "Add character",
		.onClick = [](GestureDetector::Event event) {
			event.widget.addOverlay(CharacterDataSelector{
				.onSelect = [](Character::DataKey dataKey) {
					const auto &characterData = Character::list.at(dataKey);

					++Store::lastWeaponId;
					Store::weapons.emplace(Store::lastWeaponId, Weapon::Instance(Weapon::defaultWeapons.at(characterData.baseStats.weaponType), {Store::lastWeaponId}));

					++Store::lastCharacterId;
					Store::characters.emplace(Store::lastCharacterId, Character::Instance({Store::lastCharacterId}, dataKey, {Store::lastWeaponId}));

					Store::weaponListUpdateEvent.notify();
					Store::characterListUpdateEvent.notify();
				},
			});
		},
	};

	auto buttonBar = Row{
		.widget{.height = Size::Shrink},
		.children{
			characterAddButton,
		},
	};

	return Navigator{
		.controller = controller,
		.child = ScrollableFrame{
			.scrollableWidget{
				.padding = 8.f,
			},
			.alignment = Scrollable::Alignment::center,
			.spacing = 8.f,
			.children{
				buttonBar,
				Grid{
					.widget{
						.height = Size::Shrink,
						.sizeConstraints{
							.maxWidth = 1520.f,
						},
						.onInit = [controller](Widget &w) {
							observe(w, Store::characterListUpdateEvent, [controller, &w]() {
								w.setChildren(makeCharacters(controller));
							});
							w.setChildren(makeCharacters(controller));
						},
					},
					.spacing = 2.f,
					.columnCount = Grid::MinSize{.value = 256.f},
				}
			},
		},
	};
}
