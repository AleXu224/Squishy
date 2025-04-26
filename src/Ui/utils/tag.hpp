#pragma once

#include "widgetArgs.hpp"

namespace UI {
	struct Tag {
		// Args
		std::string sourceStr;

		operator squi::Child() const;
	};
}// namespace UI