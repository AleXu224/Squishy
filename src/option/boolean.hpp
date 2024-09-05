#pragma once

#include "string_view"
#include "utils/hashedString.hpp"


namespace Option {
	struct Boolean {
		Utils::HashedString key;
		std::string_view name;
		bool active = false;

		void toggle() {
			active = !active;
		}
	};
}// namespace Option