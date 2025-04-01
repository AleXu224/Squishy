#pragma once

#include "widget.hpp"

#include "character/key.hpp"
#include "combo/option.hpp"
#include "formula/formulaContext.hpp"

namespace UI {
	struct OptionPicker {
		// Args
		squi::Widget::Args widget{};
		Character::InstanceKey characterKey;
		const Formula::Context &ctx;
		std::function<void(::Combo::Option)> onSelect;

		operator squi::Child() const;
	};
}// namespace UI