#include "discPage.hpp"

#include "UI/disc/discEditor.hpp"
#include "discCard.hpp"
#include "store.hpp"
#include "widgets/button.hpp"
#include "widgets/column.hpp"
#include "widgets/container.hpp"
#include "widgets/expander.hpp"
#include "widgets/grid.hpp"
#include "widgets/liteFilter.hpp"
#include "widgets/navigator.hpp"
#include "widgets/paginator.hpp"
#include "widgets/row.hpp"
#include "widgets/scrollview.hpp"
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
					Container{
						.widget{
							.height = Size::Wrap,
							.alignment = Alignment::CenterRight,
						},
						.child = action,
					},
				},
			};
		}
	};
}// namespace

[[nodiscard]] std::vector<Disc::Instance *> UI::DiscPage::State::getFilteredList() const {
	std::vector<Disc::Instance *> ret;

	std::vector<Stat> requiredSubStats{};
	bool allEnabled = true;
	for (const auto &[subStat, enabled]: subStatFilter) {
		if (enabled) {
			requiredSubStats.push_back(subStat);
		} else {
			allEnabled = false;
		}
	}

	for (auto &[_, disc]: ::Store::discs) {
		if (!partitionFilter.at(disc.partition)) continue;
		if (!mainStatFilter.at(disc.mainStat)) continue;
		if (!allEnabled) {
			bool subStatFound = false;
			for (const auto &requiredSubStat: requiredSubStats) {
				bool hasSubStat = false;
				for (const auto &subStat: disc.subStats) {
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
		ret.emplace_back(&disc);
	}

	return ret;
}

squi::Child UI::DiscPage::State::build(const squi::Element &element) {
	auto addDiscButton = Button{
		.theme = Button::Theme::Accent(element),
		.onClick = [this]() {
			Navigator::of(*this->element).pushOverlay(DiscEditor{
				.onSubmit = [](Disc::Instance disc) {
					disc.key = Disc::InstanceKey{++Store::lastDiscId};
					disc.updateStats();
					Store::discs.insert({disc.key, disc});
					Store::discListUpdateEvent.notify();
				},
			});
		},
		.child = "Add disc",
	};

	Child slotFilter = LiteFilter{
		.items = [&]() {
			std::vector<LiteFilter::Item> ret;

			ret.reserve(Disc::partitions.size());
			for (const auto &partition: Disc::partitions) {
				ret.emplace_back(LiteFilter::Item{
					.name = Utils::Stringify(partition),
					.onUpdate = [this, partition](bool active) {
						auto &status = this->partitionFilter.at(partition);
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

			ret.reserve(Stats::Disc::mainStats.size());
			for (const auto &stat: Stats::Disc::mainStats) {
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

			ret.reserve(Stats::Disc::subStats.size());
			for (const auto &stat: Stats::Disc::subStats) {
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
						.theme = Button::Theme::Accent(element),
						.onClick = [&element]() {
							Navigator::of(element).pushOverlay(DiscEditor{
								.onSubmit = [](Disc::Instance disc) {
									disc.key = Disc::InstanceKey{++Store::lastDiscId};
									disc.updateStats();
									Store::discs.insert({disc.key, disc});
									Store::discListUpdateEvent.notify();
								},
							});
						},
						.child = "Add disc",
					},
				},
				.content = Column{
					.children{
						ExpanderItem{.heading = "Slot", .action = slotFilter},
						ExpanderItem{
							.heading = "Main stat",
							.action = ScrollView{
								.widget{.width = Size::Wrap, .height = Size::Shrink},
								.scrollWidget{.width = Size::Wrap},
								.alignment = Flex::Alignment::center,
								.direction = Axis::Horizontal,
								.children{mainStatFilter},
							},
						},
						ExpanderItem{
							.heading = "Sub stat",
							.action = ScrollView{
								.widget{.width = Size::Wrap, .height = Size::Shrink},
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
						ret.emplace_back(UI::DiscCard{
							.key = IndexKey{(*it)->key.key},
							.disc = **it,
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
