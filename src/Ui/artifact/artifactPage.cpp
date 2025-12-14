#include "artifactPage.hpp"

#include "Ui/artifact/artifactEditor.hpp"
#include "artifactCard.hpp"
#include "store.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/expander.hpp"
#include "widgets/grid.hpp"
#include "widgets/liteFilter.hpp"
#include "widgets/navigator.hpp"
#include "widgets/paginator.hpp"
#include "widgets/row.hpp"
#include "widgets/scrollview.hpp"
#include "widgets/stack.hpp"
#include "widgets/text.hpp"


using namespace squi;

namespace {
	struct ExpanderItem : StatelessWidget {
		// Args
		Key key;
		std::string heading;
		Child action;

		[[nodiscard]] Child build(const Element &) {
			return Row{
				.widget{
					.sizeConstraints = BoxConstraints{.minHeight = 64.f},
					.padding = Padding{16.f, 0.f},
				},
				.crossAxisAlignment = Flex::Alignment::center,
				.spacing = 8.f,
				.children{
					Text{
						.widget{.width = Size::Shrink},
						.text = heading,
					},
					Stack{},
					action,
				},
			};
		}
	};
}// namespace

[[nodiscard]] std::vector<Artifact::Instance *> UI::ArtifactPage::State::getFilteredList() const {
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

squi::Child UI::ArtifactPage::State::build(const squi::Element &element) {
	auto addArtifactButton = Button{
		.theme = Button::Theme::Accent(),
		.onClick = [this]() {
			Navigator::of(*this->element).pushOverlay(ArtifactEditor{
				.onSubmit = [](Artifact::Instance artifact) {
					artifact.key = Artifact::InstanceKey{++Store::lastArtifactId};
					artifact.updateStats();
					Store::artifacts.insert({artifact.key, artifact});
					Store::artifactListUpdateEvent.notify();
				},
			});
		},
		.child = "Add artifact",
	};

	auto slotFilter = LiteFilter{
		.items = [&]() {
			std::vector<LiteFilter::Item> ret;

			ret.reserve(Artifact::slots.size());
			for (const auto &slot: Artifact::slots) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(slot),
					.onUpdate = [this, slot](bool active) {
						auto &status = this->slotFilter.at(slot);
						if (status != active) {
							setState([&]() {
								status = active;
							});
						}
					},
				});
			}

			return ret;
		}(),
	};

	auto mainStatFilter = LiteFilter{
		.items = [&]() {
			std::vector<LiteFilter::Item> ret;

			ret.reserve(Stats::Artifact::mainStats.size());
			for (const auto &stat: Stats::Artifact::mainStats) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(stat),
					.onUpdate = [this, stat](bool active) {
						auto &status = this->mainStatFilter.at(stat);
						if (status != active) {
							setState([&]() {
								status = active;
							});
						}
					},
				});
			}

			return ret;
		}(),
	};

	auto subStatFilter = LiteFilter{
		.items = [&]() {
			std::vector<LiteFilter::Item> ret;

			ret.reserve(Stats::Artifact::subStats.size());
			for (const auto &stat: Stats::Artifact::subStats) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(stat),
					.onUpdate = [this, stat](bool active) {
						auto &status = this->subStatFilter.at(stat);
						if (status != active) {
							setState([&]() {
								status = active;
							});
						}
					},
				});
			}

			return ret;
		}(),
	};

	return ScrollView{
		.scrollWidget{
			.padding = 8.f,
		},
		.alignment = Flex::Alignment::center,
		.spacing = 8.f,
		.children{
			Expander{
				.widget{
					.sizeConstraints = BoxConstraints{
						.maxWidth = 1520.f,
					},
				},
				.title = "Filters",
				.action{
					Button{
						.theme = Button::Theme::Accent(),
						.onClick = [&element]() {
							Navigator::of(element).pushOverlay(ArtifactEditor{
								.onSubmit = [](Artifact::Instance artifact) {
									artifact.key = Artifact::InstanceKey{++Store::lastArtifactId};
									artifact.updateStats();
									Store::artifacts.insert({artifact.key, artifact});
									Store::artifactListUpdateEvent.notify();
								},
							});
						},
						.child = "Add artifact",
					},
				},
				.content = Column{
					.children{
						ExpanderItem{.heading = "Slot", .action = slotFilter},
						ExpanderItem{
							.heading = "Main stat",
							.action = ScrollView{
								.widget{.width = Size::Wrap},
								.scrollWidget{.width = Size::Wrap},
								.alignment = Flex::Alignment::center,
								.direction = Axis::Horizontal,
								.children{mainStatFilter},
							},
						},
						ExpanderItem{
							.heading = "Sub stat",
							.action = ScrollView{
								.widget{.width = Size::Wrap},
								.scrollWidget{.width = Size::Wrap},
								.alignment = Flex::Alignment::center,
								.direction = Axis::Horizontal,
								.children{subStatFilter},
							},
						},
					},
				},
			},
			Paginator{
				.getItemCount = [this]() {
					return this->getFilteredList().size();
				},
				.builder = [this](uint32_t offset, uint32_t count) {
					Children ret;
					auto filteredList = this->getFilteredList();
					auto begin = std::next(filteredList.begin(), offset);
					auto end = std::next(begin, count);
					for (auto it = begin; it != end; it++) {
						ret.emplace_back(UI::ArtifactCard{
							.artifact = **it,
						});
					}
					return Grid{
						.widget{
							.height = Size::Shrink,
							.sizeConstraints = BoxConstraints{
								.maxWidth = 1520.f,
							},
						},
						.columnCount = Grid::MinSize{256.f},
						.spacing = 2.f,
						.children = ret,
					};
				},
			},
		},
	};
}
