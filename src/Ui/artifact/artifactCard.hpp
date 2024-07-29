#pragma once

#include "widget.hpp"
#include "artifact/instance.hpp"

namespace UI {
	struct ArtifactCard {
		// Args
		squi::Widget::Args widget{};
		Artifact::Instance &artifact;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI