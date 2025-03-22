#pragma once

#include "widget.hpp"

namespace UI {
	struct ArtifactPage {
		// Args
		squi::Widget::Args widget{};

		operator squi::Child() const;
	};
}// namespace UI