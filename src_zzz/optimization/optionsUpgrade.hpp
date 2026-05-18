#pragma once

#include "combo/source.hpp"
#include "optional"


namespace Optimization {
	struct OptionsUpgrade {
		std::optional<Combo::Source::Types> nodeSource;
	};
}// namespace Optimization