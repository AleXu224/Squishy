#pragma once

#include "node/node.hpp"
#include "option/option.hpp"
#include "stats/sheet.hpp"


namespace Disc {
	struct SetBonus {
		Option::DiscList opts{};
		Stats::ModsSheet mods{};
		Node::DiscList nodes{};
	};
}// namespace Agent