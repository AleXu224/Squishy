#pragma once

#include "widget.hpp"

#include "combo/source.hpp"
#include "functional"

namespace UI {
	struct NodePicker {
		// Args
		squi::Widget::Args widget{};
		Character::InstanceKey characterKey;
		bool enableCombos = false;
		const Formula::Context &ctx;
		std::function<void(Combo::Source::Types)> onSelect;

		operator squi::Child() const;
	};
}// namespace UI