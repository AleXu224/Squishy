#pragma once

#include "artifact/instance.hpp"
#include "cstdint"
#include "widget.hpp"


namespace UI {
	struct ArtifactCard {
		enum class Actions : uint8_t {
			list,
			character,
			showcase,
		};

		// Args
		squi::Widget::Args widget{};
		Artifact::Instance &artifact;
		Actions actions = Actions::list;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI