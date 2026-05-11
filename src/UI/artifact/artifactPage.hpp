#pragma once

#include "core/core.hpp"
#include "store.hpp"

namespace UI {
	struct ArtifactPage : squi::StatefulWidget {
		// Args
		squi::Key key;

		struct State : squi::WidgetState<ArtifactPage> {
			squi::VoidObserver artifactListUpdateObserver;
			std::unordered_map<Artifact::Slot, bool> slotFilter;
			std::unordered_map<Stat, bool> mainStatFilter;
			std::unordered_map<Stat, bool> subStatFilter;

			[[nodiscard]] std::vector<Artifact::Instance *> getFilteredList() const;

			void initState() override {
				artifactListUpdateObserver = ::Store::artifactListUpdateEvent.observe([this]() {
					this->element->markNeedsRebuild();
				});

				for (const auto &slot: Artifact::slots) {
					slotFilter[slot] = true;
				}
				for (const auto &stat: Stats::Artifact::mainStats) {
					mainStatFilter[stat] = true;
				}
				for (const auto &stat: Stats::Artifact::subStats) {
					subStatFilter[stat] = true;
				}
			}

			squi::Child build(const squi::Element &element) override;
		};
	};
}// namespace UI