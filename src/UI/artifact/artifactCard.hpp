#pragma once

#include "artifact/instance.hpp"
#include "core/core.hpp"
#include "cstdint"
#include "store.hpp"

namespace UI {
	using namespace squi;
	struct ArtifactCard : StatefulWidget {
		enum class Actions : uint8_t {
			list,
			character,
			showcase,
		};

		// Args
		Key key;
		Args widget;
		Artifact::InstanceKey artifact;
		Actions actions = Actions::list;

		struct State : WidgetState<ArtifactCard> {
			VoidObserver artifactUpdateObserver;

			void observeArtifact() {
				if (auto it = ::Store::artifacts.find(widget->artifact); it != ::Store::artifacts.end()) {
					artifactUpdateObserver = it->second.updateEvent.observe([this]() {
						this->element->markNeedsRebuild();
					});
				}
			}

			void initState() override {
				observeArtifact();
			}

			void widgetUpdated() override {
				observeArtifact();
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI