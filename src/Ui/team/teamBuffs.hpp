#pragma once

#include "core/core.hpp"

#include "team/key.hpp"

namespace UI {
	using namespace squi;
	struct TeamBuffs : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Team::InstanceKey instanceKey;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI