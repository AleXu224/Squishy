#include "weaponPage.hpp"
#include "Ui/utils/grid.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"
#include "weaponCard.hpp"


using namespace squi;

UI::WeaponPage::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return ScrollableFrame{
		.children{
			Grid{
				.widget{
					.height = Size::Shrink,
					.padding = Padding{8.f},
					.onInit = [](Widget &w) {
						for (auto &[_, weapon]: Store::weapons) {
							w.addChild(WeaponCard{
								.weapon = weapon,
							});
						}
					},
				},
				.spacing = 2.f,
				.columnCount = Grid::MinSize{.value = 256.f},
			}
		},
	};
}
