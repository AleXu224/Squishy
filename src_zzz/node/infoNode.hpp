#pragma once

#include "color.hpp"
#include "formula/node.hpp"
#include "nodeData.hpp"
#include "utils/entryType.hpp"

namespace Node {
	struct Info {
		std::string name;
		squi::Color color = squi::Color::white;
		Utils::EntryType type = Utils::EntryType::seconds;
		bool optimizable = false;
		Formula::FloatNode formula;

		Formula::FloatNode _formula = formula;

		Data _data = InfoData{
			.name = name,
			.type = type,
			.color = color,
			.optimizable = optimizable,
		};
	};
}// namespace Node