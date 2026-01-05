#include "characterList.hpp"

#include "character/characters.hpp"
#include "characterCard.hpp"
#include "characterDataSelector.hpp"
#include "store.hpp"
#include "weapon/defaultWeapons.hpp"
#include "widgets/button.hpp"
#include "widgets/grid.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/scrollview.hpp"
#include <GLFW/glfw3.h>


using namespace squi;

squi::core::Child UI::CharacterList::State::build(const Element &element) {
	auto characterAddButton = Button{
		.onClick = [this]() {
			Navigator::of(*this->element).pushOverlay(CharacterDataSelector{
				.onSelect = [](Character::DataKey dataKey) {
					const auto &characterData = Character::list.at(dataKey);

					++Store::lastWeaponId;
					Store::weapons.emplace(Store::lastWeaponId, Weapon::Instance(Weapon::defaultWeapons.at(characterData.baseStats.weaponType), {Store::lastWeaponId}));

					++Store::lastCharacterId;
					auto character = Store::characters.emplace(Store::lastCharacterId, Character::Instance({Store::lastCharacterId}, dataKey));
					character.first->second.state.equippedLoadout.swapWeapon({Store::lastWeaponId});

					Store::weaponListUpdateEvent.notify();
					Store::characterListUpdateEvent.notify();
				},
			});
		},
		.child = "Add character",
	};

	Child buttonBar = Row{
		.widget{.height = Size::Shrink},
		.children{
			characterAddButton,
		},
	};

	return ScrollView{
		.scrollWidget{
			.padding = 8.f,
		},
		.alignment = Flex::Alignment::center,
		.spacing = 8.f,
		.children{
			buttonBar,
			Grid{
				.widget{
					.height = Size::Shrink,
					.sizeConstraints = BoxConstraints{
						.maxWidth = 1520.f,
					},
				},
				.columnCount = Grid::MinSize{.value = 256.f},
				.spacing = 2.f,
				.children = [&]() {
					Children ret;
					for (auto &[_, character]: ::Store::characters) {
						ret.emplace_back(UI::CharacterCard{
							.characterKey = character.instanceKey,
						});
					}
					return ret;
				}(),
			}
		},
	};
}
