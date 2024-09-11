#pragma once

#include "formula/formulaContext.hpp"
#include "widget.hpp"


namespace UI {
	struct CharacterTransformativeReactions {
		// Args
		squi::Widget::Args widget{};
		Formula::Context ctx;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI