#include "artifactPage.hpp"

#include "Ui/artifact/artifactEditor.hpp"
#include "Ui/utils/grid.hpp"
#include "artifactCard.hpp"
#include "button.hpp"
#include "row.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"
#include "widgets/paginator.hpp"

using namespace squi;

namespace {
	[[nodiscard]] auto makeArtifacts(uint32_t offset, uint32_t count) {
		Children ret;
		auto begin = std::next(::Store::artifacts.begin(), offset);
		auto end = std::next(begin, count);
		for (auto it = begin; it != end; it++) {
			ret.emplace_back(UI::ArtifactCard{
				.artifact = it->second,
			});
		}
		return ret;
	}
}// namespace

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
		.alignment = squi::Column::Alignment::center,
		.spacing = 8.f,
		.children{
			buttonBar,
			Paginator{
				.refreshItemsEvent = ::Store::artifactListUpdateEvent,
				.getItemCount = []() {
					return ::Store::artifacts.size();
				},
				.builder = [](uint32_t offset, uint32_t count) {
					return Grid{
						.widget{
							.height = Size::Shrink,
							.sizeConstraints{
								.maxWidth = 1520.f,
							},
						},
						.spacing = 2.f,
						.columnCount = Grid::MinSize{256.f},
						.children = makeArtifacts(offset, count),
					};
				},
			},
		},
	};
}
