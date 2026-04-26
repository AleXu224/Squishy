#pragma once

#include "core/core.hpp"
#include "store.hpp"

namespace UI {
	struct DiscPage : squi::StatefulWidget {
		// Args
		squi::Key key;

		struct State : squi::WidgetState<DiscPage> {
			squi::VoidObserver discListUpdateObserver;
			std::unordered_map<Disc::Partition, bool> partitionFilter;
			std::unordered_map<Stat, bool> mainStatFilter;
			std::unordered_map<Stat, bool> subStatFilter;

			[[nodiscard]] std::vector<Disc::Instance *> getFilteredList() const;

			void initState() override {
				discListUpdateObserver = ::Store::discListUpdateEvent.observe([this]() {
					this->element->markNeedsRebuild();
				});

				for (const auto &partition: Disc::partitions) {
					partitionFilter[partition] = true;
				}
				for (const auto &stat: Stats::Disc::mainStats) {
					mainStatFilter[stat] = true;
				}
				for (const auto &stat: Stats::Disc::subStats) {
					subStatFilter[stat] = true;
				}
			}

			squi::Child build(const squi::Element &element) override;
		};
	};
}// namespace UI