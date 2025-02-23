#pragma once

#include "character/key.hpp"
#include "option/valueList.hpp"
#include "widgetArgs.hpp"


namespace UI {
	struct ValueListOption {
		// Args
		Option::ValueList &option;
		Character::InstanceKey characterKey;
		const Formula::Context &ctx;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI