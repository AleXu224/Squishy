#include "artifactPage.hpp"

#include "Ui/artifact/artifactEditor.hpp"
#include "Ui/utils/grid.hpp"
#include "artifactCard.hpp"
#include "button.hpp"
#include "column.hpp"
#include "container.hpp"
#include "expander.hpp"
#include "row.hpp"
#include "scrollable.hpp"
#include "scrollableFrame.hpp"
#include "store.hpp"
#include "text.hpp"
#include "widgets/liteFilter.hpp"
#include "widgets/paginator.hpp"


using namespace squi;

namespace {
	struct ArtifactPageState {
		// Data
		std::unordered_map<Artifact::Slot, bool> slotFilter;
		std::unordered_map<Stat, bool> mainStatFilter;
		std::unordered_map<Stat, bool> subStatFilter;

		std::vector<Artifact::Instance *> getFilteredList() const {
			std::vector<Artifact::Instance *> ret;

			for (auto &[_, artifact]: ::Store::artifacts) {
				if (!slotFilter.at(artifact.slot)) continue;
				if (!mainStatFilter.at(artifact.mainStat)) continue;
				bool subStatFound = false;
				for (const auto &substat: artifact.subStats) {
					if (!substat.stat) continue;
					if (subStatFilter.at(substat.stat.value())) subStatFound = true;
				}
				if (!subStatFound) continue;
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
	struct ExpanderItem {
		// Args
		std::string_view heading;
		Child action;

		operator squi::Child() const {
			return Row{
				.widget{
					.sizeConstraints{.minHeight = 64.f},
					.padding = Padding{16.f, 0.f},
				},
				.alignment = squi::Row::Alignment::center,
				.spacing = 8.f,
				.children{
					Text{
						.widget{.width = Size::Shrink},
						.text = heading,
					},
					Container{},
					action,
				},
			};
		}
	};
}// namespace

UI::ArtifactPage::operator squi::Child() const {
	auto storage = std::make_shared<ArtifactPageState>();
	for (const auto &slot: Artifact::slots) {
		storage->slotFilter[slot] = true;
	}
	for (const auto &stat: Stats::Artifact::mainStats) {
		storage->mainStatFilter[stat] = true;
	}
	for (const auto &stat: Stats::Artifact::subStats) {
		storage->subStatFilter[stat] = true;
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

	auto slotFilter = LiteFilter{
		.widget{
			.width = Size::Shrink,
			.height = Size::Shrink,
		},
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

	auto mainStatFilter = LiteFilter{
		.widget{
			.width = Size::Shrink,
			.height = Size::Shrink,
		},
		.multiSelect = true,
		.items = [storage]() {
			std::vector<LiteFilter::Item> ret;

			for (const auto &stat: Stats::Artifact::mainStats) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(stat),
					.onUpdate = [storage, stat](bool active) {
						auto &status = storage->mainStatFilter.at(stat);
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

	auto subStatFilter = LiteFilter{
		.widget{
			.width = Size::Shrink,
			.height = Size::Shrink,
		},
		.multiSelect = true,
		.items = [storage]() {
			std::vector<LiteFilter::Item> ret;

			for (const auto &stat: Stats::Artifact::subStats) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(stat),
					.onUpdate = [storage, stat](bool active) {
						auto &status = storage->subStatFilter.at(stat);
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
		.alignment = Scrollable::Alignment::center,
		.spacing = 8.f,
		.children{
			Expander{
				.widget{
					.sizeConstraints{
						.maxWidth = 1520.f,
					},
				},
				.heading = "Filters",
				.actions{
					addArtifactButton,
				},
				.expandedContent = Column{
					.children{
						ExpanderItem{
							.heading = "Slot",
							.action = slotFilter
						},
						ExpanderItem{
							.heading = "Main stat",
							.action = ScrollableFrame{
								.widget{.width = Size::Wrap},
								.scrollableWidget{.width = Size::Wrap},
								.alignment = Scrollable::Alignment::center,
								.direction = Scrollable::Direction::horizontal,
								.children{mainStatFilter},
							},
						},
						ExpanderItem{
							.heading = "Sub stat",
							.action = ScrollableFrame{
								.widget{.width = Size::Wrap},
								.scrollableWidget{.width = Size::Wrap},
								.alignment = Scrollable::Alignment::center,
								.direction = Scrollable::Direction::horizontal,
								.children{subStatFilter},
							},
						},
					},
				},
			},
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
