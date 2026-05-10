#pragma once

#include "formula/node.hpp"
#include "node/node.hpp"
#include "stats/sheet.hpp"
#include "string_view"
#include "utils/hashedString.hpp"
#include <vector>


namespace Option {
	struct ValueSlider {
		Utils::HashedString key;
		std::string_view name;
		bool teamBuff = false;
		Formula::BoolNode displayCondition{};
		std::vector<float> values;
		float value = values.empty() ? 0.f : values.front();
		Stats::ModsSheet mods{};
		std::vector<Node::Types> nodes{};

		[[nodiscard]] float getValue() const {
			return value;
		}
	};
}// namespace Option
