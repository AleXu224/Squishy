#pragma once

#include "disc/key.hpp"
#include "combo/source.hpp"
#include "optional"


namespace Optimization {
	struct OptionsTc {
		std::optional<Combo::Source::Types> nodeSource;
		Disc::SetKey twoPcSet{};
		Disc::SetKey fourPcSet{};
	};
}// namespace Optimization