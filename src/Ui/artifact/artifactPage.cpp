#include "artifactPage.hpp"

#include "Ui/artifact/artifactEditor.hpp"
#include "Ui/utils/grid.hpp"
#include "artifactCard.hpp"
#include "button.hpp"
#include "row.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"
#include "widgets/liteFilter.hpp"
#include "widgets/paginator.hpp"

using namespace squi;

namespace {
	struct ArtifactPageState {
		// Data
		std::unordered_map<Artifact::Slot, bool> slotFilter;

		std::vector<Artifact::Instance *> getFilteredList() const {
			std::vector<Artifact::Instance *> ret;

			for (auto &[_, artifact]: ::Store::artifacts) {
				if (!slotFilter.at(artifact.slot)) continue;
				ret.emplace_back(&artifact);
			}

			return ret;
		}
	};
	[[nodiscard]] auto makeArtifacts(uint32_t offset, uint32_t count, std::shared_ptr<ArtifactPageState> storage) {
		Children ret;
		auto filteredList = storage->getFilteredList();
		auto begin = std::next(filteredList.begin(), offset);
		auto end = std::next(begin, count);
		for (auto it = begin; it != end; it++) {
			ret.emplace_back(UI::ArtifactCard{
				.artifact = **it,
			});
		}
		return ret;
	}
}// namespace

UI::ArtifactPage::operator squi::Child() const {
	auto storage = std::make_shared<ArtifactPageState>();
	for (const auto &slot: Artifact::slots) {
		storage->slotFilter[slot] = true;
	}

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

	auto slotFilter = LiteFilter{
		.multiSelect = true,
		.items = [storage]() {
			std::vector<LiteFilter::Item> ret;

			for (const auto &slot: Artifact::slots) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(slot),
					.onUpdate = [storage, slot](bool active) {
						auto &status = storage->slotFilter.at(slot);
						if (status != active) {
							status = active;
							::Store::artifactListUpdateEvent.notify();
						}
					},
				});
			}

			return ret;
		}(),
	};

	return ScrollableFrame{
		.scrollableWidget{
			.padding = 8.f,
		},
		.alignment = squi::Column::Alignment::center,
		.spacing = 8.f,
		.children{
			slotFilter,
			buttonBar,
			Paginator{
				.refreshItemsEvent = ::Store::artifactListUpdateEvent,
				.getItemCount = [storage]() {
					return storage->getFilteredList().size();
				},
				.builder = [storage](uint32_t offset, uint32_t count) {
					return Grid{
						.widget{
							.height = Size::Shrink,
							.sizeConstraints{
								.maxWidth = 1520.f,
							},
						},
						.spacing = 2.f,
						.columnCount = Grid::MinSize{256.f},
						.children = makeArtifacts(offset, count, storage),
					};
				},
			},
		},
	};
}
