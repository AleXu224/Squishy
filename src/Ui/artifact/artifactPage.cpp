#include "artifactPage.hpp"
#include "Ui/utils/grid.hpp"
#include "artifactCard.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"

using namespace squi;

UI::ArtifactPage::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	return ScrollableFrame{
		.children{
			Grid{
				.widget{
					.height = Size::Shrink,
					.padding = Padding{8.f},
					.onInit = [](Widget &w) {
						for (auto &[_, artifact]: Store::artifacts) {
							w.addChild(ArtifactCard{
								.artifact = artifact,
							});
						}
					},
				},
				.spacing = 2.f,
				.columnCount = Grid::MinSize{256.f},
			},
		},
	};
}
