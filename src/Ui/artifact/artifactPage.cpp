#include "artifactPage.hpp"

#include "Ui/artifact/artifactEditor.hpp"
#include "Ui/utils/grid.hpp"
#include "artifactCard.hpp"
#include "button.hpp"
#include "row.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"

using namespace squi;

UI::ArtifactPage::operator squi::Child() const {
	auto storage = std::make_shared<Storage>();

	auto addArtifactButton = Button{
		.text = "Add artifact",
		.style = ButtonStyle::Accent(),
		.onClick = [](GestureDetector::Event event) {
			event.widget.addOverlay(ArtifactEditor{
				.onSubmit = [](Artifact::Instance artifact) {
					artifact.key = Artifact::InstanceKey{++Store::lastArtifactId};
					artifact.updateStats();
					Store::artifacts.insert({artifact.key, artifact});
					Store::artifactListUpdateEvent.notify();
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
						constexpr auto makeArtifacts = []() {
							Children ret;
							for (auto &[_, artifact]: ::Store::artifacts) {
								ret.emplace_back(ArtifactCard{
									.artifact = artifact,
								});
							}
							return ret;
						};
						w.setChildren(makeArtifacts());

						w.customState.add(Store::artifactListUpdateEvent.observe([&w, makeArtifacts]() {
							w.setChildren(makeArtifacts());
						}));
					},
				},
				.spacing = 2.f,
				.columnCount = Grid::MinSize{256.f},
			},
		},
	};
}
