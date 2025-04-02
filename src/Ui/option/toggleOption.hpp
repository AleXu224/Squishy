#pragma once

#include "widget.hpp"

#include "character/key.hpp"
#include "option/boolean.hpp"
#include "team/key.hpp"


namespace UI {
	struct ToggleOption {
		// Args
		squi::Widget::Args widget{};
		Option::Boolean &option;
		std::variant<Character::InstanceKey, Team::InstanceKey> instanceKey{};
		squi::Observable<bool> switchEvent{};
		Formula::Context ctx;

		struct Storage {
			// Data
			bool active;
		};

		operator squi::Child() const;
	};
}// namespace UI