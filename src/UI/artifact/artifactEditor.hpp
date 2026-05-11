#pragma once

#include "core/core.hpp"

#include "artifact/instance.hpp"

namespace UI {
	using namespace squi;
	struct ArtifactEditor : StatefulWidget {
		// Args
		Key key;
		std::optional<Artifact::Instance> artifact{};
		std::function<void(Artifact::Instance)> onSubmit{};

		struct State : WidgetState<ArtifactEditor> {
			Artifact::Instance artifact{};
			VoidObservable closeEvent{};

			void updateSlot(Artifact::Slot newSlot);
			Child createSubStat(size_t subStatIndex);

			void initState() override;

			Child build(const Element &element) override;
		};
	};
}// namespace UI