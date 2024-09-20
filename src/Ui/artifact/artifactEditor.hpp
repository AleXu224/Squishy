#pragma once

#include "widget.hpp"

#include "artifact/instance.hpp"

namespace UI {
	struct ArtifactEditor {
		// Args
		squi::Widget::Args widget{};
		std::optional<Artifact::Instance> artifact;

		struct Storage {
			// Data
			Artifact::Instance artifact{};
		};

		operator squi::Child() const;
	};
}// namespace UI