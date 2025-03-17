#pragma once

#include "stats/stat.hpp"
#include "widgetArgs.hpp"

namespace UI {
	struct StatDisplay {
		// Args
		bool isTransparent;
		StatValue stat;

		operator squi::Child() const;
	};

}// namespace UI