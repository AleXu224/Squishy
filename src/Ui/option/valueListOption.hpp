#pragma once

#include "character/key.hpp"
#include "option/valueList.hpp"
#include "team/key.hpp"
#include "widgetArgs.hpp"


namespace UI {
	struct ValueListOption {
		// Args
		Option::ValueList &option;
		std::variant<Character::InstanceKey, Team::InstanceKey> instanceKey{};
		const Formula::Context &ctx;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI