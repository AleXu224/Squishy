#include "characterPage.hpp"

#include "Ui/utils/grid.hpp"
#include "characterCard.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"

using namespace squi;

UI::CharacterPage::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return ScrollableFrame{
		.children{
			Grid{
				.widget{
					.height = Size::Shrink,
					.padding = Padding{8.f},
					.onInit = [](Widget &w) {
						for (auto &[_, character]: Store::characters) {
							w.addChild(CharacterCard{
								.character = character,
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
