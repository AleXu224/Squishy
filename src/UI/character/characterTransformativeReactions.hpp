#pragma once

#include "core/core.hpp"
#include "formula/base.hpp"


namespace UI {
	using namespace squi;
	struct CharacterTransformativeReactions : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Formula::Context ctx;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI