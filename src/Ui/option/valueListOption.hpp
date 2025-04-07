#pragma once

#include "widget.hpp"

#include "character/key.hpp"
#include "observer.hpp"
#include "option/valueList.hpp"
#include "team/key.hpp"
#include "widgetArgs.hpp"


namespace UI {
	struct ValueListOption {
		// Args
		squi::Widget::Args widget{};
		Option::ValueList &option;
		std::variant<Character::InstanceKey, Team::InstanceKey> instanceKey{};
		squi::Observable<std::optional<uint32_t>, std::optional<uint32_t>> valueChangedEvent{};
		Formula::Context ctx;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI