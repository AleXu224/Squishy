#pragma once

#include "artifact/instance.hpp"
#include "widget.hpp"


namespace UI {
	struct ArtifactCard {
		// Args
		squi::Widget::Args widget{};
		Artifact::Instance &artifact;
		bool hasActions = true;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI