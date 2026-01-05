#pragma once

#include "character/key.hpp"
#include "core/core.hpp"
#include "formula/formulaContext.hpp"


namespace UI {
	using namespace squi;
	struct ComboDisplay : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey;
		Formula::Context ctx;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI