#include "artifactPage.hpp"

#include "UI/artifact/artifactEditor.hpp"
#include "artifactCard.hpp"
#include "store.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/expander.hpp"
#include "widgets/grid.hpp"
#include "widgets/liteFilter.hpp"
#include "widgets/navigator.hpp"
#include "widgets/paginator.hpp"
#include "widgets/scrollview.hpp"


using namespace squi;

[[nodiscard]] std::vector<Artifact::Instance *> UI::ArtifactPage::State::getFilteredList() const {
	std::vector<Artifact::Instance *> ret;

	std::vector<Stat> requiredSubStats{};
	bool allEnabled = true;
	for (const auto &[subStat, enabled]: subStatFilter) {
		if (enabled) {
			requiredSubStats.push_back(subStat);
		} else {
			allEnabled = false;
		}
	}

	for (auto &[_, artifact]: ::Store::artifacts) {
		if (!slotFilter.at(artifact.slot)) continue;
		if (!mainStatFilter.at(artifact.mainStat)) continue;
		if (!allEnabled) {
			bool subStatFound = false;
			for (const auto &requiredSubStat: requiredSubStats) {
				bool hasSubStat = false;
				for (const auto &subStat: artifact.subStats) {
					if (subStat.stat == requiredSubStat) {
						hasSubStat = true;
						break;
					}
				}
				if (!hasSubStat) {
					subStatFound = false;
					break;
				}
				subStatFound = true;
			}
			if (!subStatFound) continue;
		}
		ret.emplace_back(&artifact);
	}

	return ret;
}

squi::Child UI::ArtifactPage::State::build(const squi::Element &element) {
	auto addArtifactButton = Button{
		.theme = Button::Theme::Accent(element),
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

	Child slotFilter = LiteFilter{
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

	Child mainStatFilter = LiteFilter{
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

	Child subStatFilter = LiteFilter{
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
					.sizeConstraints{
						.maxWidth = 1520.f,
					},
				},
				.title = "Filters",
				.action{
					Button{
						.theme = Button::Theme::Accent(element),
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
					.spacer = ExpanderItemSpacer{},
					.children{
						ExpanderItem{.title = "Slot", .action = slotFilter},
						ExpanderItem{
							.title = "Main stat",
							.action = ScrollView{
								.widget{.width = Size::Wrap, .height = Size::Shrink},
								.alignment = Flex::Alignment::center,
								.direction = Axis::Horizontal,
								.children{mainStatFilter},
							},
						},
						ExpanderItem{
							.title = "Sub stat",
							.action = ScrollView{
								.widget{.width = Size::Wrap, .height = Size::Shrink},
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
							.key = IndexKey{(*it)->key.key},
							.artifact = (**it).key,
						});
					}
					return Grid{
						.widget{
							.height = Size::Shrink,
							.sizeConstraints{
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
