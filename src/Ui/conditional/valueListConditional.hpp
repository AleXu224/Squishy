#pragma once

#include "widget.hpp"
#include "conditional/valueList.hpp"
#include "character/key.hpp"

namespace UI {
	struct ValueListConditional {
		// Args
        Conditional::ValueList &conditional;
		Character::Key characterKey;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI