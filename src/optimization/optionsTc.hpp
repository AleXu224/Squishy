#pragma once

#include "artifact/key.hpp"
#include "combo/source.hpp"
#include "optional"


namespace Optimization {
	struct OptionsTc {
		std::optional<Combo::Source::Types> nodeSource;
		Artifact::SetKey twoPcSet{};
		Artifact::SetKey fourPcSet{};
	};
}// namespace Optimization