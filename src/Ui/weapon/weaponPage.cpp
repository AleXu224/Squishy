#include "weaponPage.hpp"
#include "Ui/utils/grid.hpp"
#include "Ui/weapon/weaponEditor.hpp"
#include "Ui/weapon/weaponSelector.hpp"
#include "button.hpp"
#include "row.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"
#include "weaponCard.hpp"


using namespace squi;

namespace {
	[[nodiscard]] auto makeWeapons() {
		Children ret;
		for (auto &[_, weapon]: ::Store::weapons) {
			ret.emplace_back(UI::WeaponCard{
				.weapon = weapon,
			});
		}
		return ret;
	}
}// namespace

UI::WeaponPage::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	auto addArtifactButton = Button{
		.text = "Add weapon",
		.style = ButtonStyle::Accent(),
		.onClick = [](GestureDetector::Event event) {
			event.widget.addOverlay(WeaponSelector{
				.onSelect = [&w = event.widget](Weapon::DataKey key) {
					Weapon::Instance instance{key, {++Store::lastWeaponId}};
					auto &weapon = Store::weapons.insert({instance.instanceKey, instance}).first->second;
					Store::weaponListUpdateEvent.notify();

					w.addOverlay(WeaponEditor{
						.weapon = weapon,
						.onSubmit = [](const Weapon::Instance &weapon) {
							auto &instance = Store::weapons.at(weapon.instanceKey);

							instance.stats = weapon.stats;
							instance.updateEvent.notify();
							for (const auto &[_, character]: Store::characters) {
								if (character.weaponInstanceKey == weapon.instanceKey) {
									character.updateEvent.notify();
								}
							}
						},
					});
				},
			});
		},
	};

	auto buttonBar = Row{
		.widget{.height = Size::Shrink},
		.children{
			addArtifactButton,
		},
	};

	return ScrollableFrame{
		.scrollableWidget{
			.padding = 8.f,
		},
		.spacing = 8.f,
		.children{
			buttonBar,
			Grid{
				.widget{
					.height = Size::Shrink,
					.onInit = [](Widget &w) {
						w.setChildren(makeWeapons());

						observe(w, Store::weaponListUpdateEvent, [&w]() {
							w.setChildren(makeWeapons());
						});
					},
				},
				.spacing = 2.f,
				.columnCount = Grid::MinSize{.value = 256.f},
			}
		},
	};
}
