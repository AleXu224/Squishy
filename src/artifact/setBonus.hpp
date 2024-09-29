#pragma once

#include "option/option.hpp"
#include "stats/sheet.hpp"
#include "node/node.hpp"

namespace Artifact {
	struct SetBonus {
		Option::ArtifactList opts{};
		Stats::ModsSheet mods{};
		Node::ArtifactList nodes{};
	};
}// namespace Artifact