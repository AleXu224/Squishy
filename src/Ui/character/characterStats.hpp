#pragma once

#include "formula/formulaContext.hpp"

#include "widget.hpp"

namespace UI {
	struct CharacterStats {
		// Args
		squi::Widget::Args widget{};
		Formula::Context ctx;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI