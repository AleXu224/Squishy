#pragma once

#include "string_view"
#include "utils/hashedString.hpp"


namespace Conditional {
	struct Boolean {
		Utils::HashedString key;
		std::string_view name;
		bool active = false;

		void toggle() {
			active = !active;
		}
	};
}// namespace Conditional