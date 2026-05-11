#pragma once

#include "artifact/instance.hpp"
#include "core/core.hpp"
#include "cstdint"


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
		Artifact::Instance &artifact;
		Actions actions = Actions::list;

		struct State : WidgetState<ArtifactCard> {
			VoidObserver artifactUpdateObserver;

			void initState() override {
				artifactUpdateObserver = widget->artifact.updateEvent.observe([this]() {
					this->element->markNeedsRebuild();
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI