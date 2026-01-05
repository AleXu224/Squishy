#pragma once

#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct SkillHeader : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		std::string name;
		std::string subtitle{};

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI