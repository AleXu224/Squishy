#pragma once

#include "formula/node.hpp"
#include "node/node.hpp"
#include "stats/sheet.hpp"
#include "string_view"
#include "utils/hashedString.hpp"


namespace Option {
	struct Boolean {
		Utils::HashedString key;
		std::string_view name;
		bool teamBuff = false;
		Formula::BoolNode displayCondition{};
		bool active = false;
		Stats::ModsSheet mods{};
		std::vector<Node::Types> nodes{};

		void toggle() {
			active = !active;
		}
	};
}// namespace Option