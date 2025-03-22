#pragma once

#include "widget.hpp"

#include "artifact/key.hpp"
#include "character/instance.hpp"

namespace UI {
	struct OptimizationSetChooser {
		// Args
		squi::Widget::Args widget{};
		Character::Instance &character;
        Formula::Context ctx;
		std::unordered_map<Artifact::SetKey, bool> &twoPcSets;
		std::unordered_map<Artifact::SetKey, bool> &fourPcSets;

		operator squi::Child() const;
	};
}// namespace UI