#pragma once

#include "character/key.hpp"
#include "option/valueList.hpp"
#include "widget.hpp"


namespace UI {
	struct ValueListOption {
		// Args
		Option::ValueList &option;
		Character::InstanceKey characterKey;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI