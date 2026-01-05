#include "weaponPage.hpp"
#include "Ui/weapon/weaponEditor.hpp"
#include "Ui/weapon/weaponSelector.hpp"
#include "store.hpp"
#include "weapon/data.hpp"
#include "weaponCard.hpp"
#include "widgets/button.hpp"
#include "widgets/grid.hpp"
#include "widgets/navigator.hpp"
#include "widgets/row.hpp"
#include "widgets/scrollview.hpp"


using namespace squi;

squi::core::Child UI::WeaponPage::State::build(const Element &element) {
	auto addArtifactButton = Button{
		.theme = Button::Theme::Accent(),
		.onClick = [this]() {
			Navigator::of(*this->element).pushOverlay(WeaponSelector{
				.onSelect = [this](Weapon::DataKey key) {
					Weapon::Instance instance{key, {++Store::lastWeaponId}};
					auto &weapon = Store::weapons.insert({instance.instanceKey, instance}).first->second;
					Store::weaponListUpdateEvent.notify();

					Navigator::of(*this->element).pushOverlay(WeaponEditor{
						.weapon = weapon,
						.onSubmit = [](const Weapon::Instance &weapon) {
							auto &instance = Store::weapons.at(weapon.instanceKey);

							instance.stats = weapon.stats;
							instance.updateEvent.notify();
							for (auto &[_, character]: Store::characters) {
								if (!instance.isUsedOn(character.instanceKey)) continue;
								instance.stats.data->getOpts(character.state.options);
								character.updateEvent.notify();
							}
						},
					});
				},
			});
		},
		.child = "Add weapon",
	};

	auto buttonBar = Row{
		.widget{.height = Size::Shrink},
		.children{
			addArtifactButton,
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
				.children = []() {
					Children ret;
					for (auto &[_, weapon]: ::Store::weapons) {
						ret.emplace_back(UI::WeaponCard{
							.weapon = weapon,
						});
					}
					return ret;
				}(),
			}
		},
	};
}
