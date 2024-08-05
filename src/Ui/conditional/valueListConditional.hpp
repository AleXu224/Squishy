#pragma once

#include "widget.hpp"
#include "conditional/valueList.hpp"

namespace UI {
	struct ValueListConditional {
		// Args
        Conditional::ValueList &conditional;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI