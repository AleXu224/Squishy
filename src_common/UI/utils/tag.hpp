#pragma once

#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct Tag : StatelessWidget {
		// Args
		Key key;
		std::string sourceStr;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI