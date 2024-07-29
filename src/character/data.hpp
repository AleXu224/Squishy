#pragma once

#include "Node.hpp"
#include "stats/characterBase.hpp"
#include "string_view"


namespace Character {
	struct Instance;

	struct Nodes {
		const std::vector<Node> normal;
		const std::vector<Node> charged;
		const std::vector<Node> plunge;
		const std::vector<Node> skill;
		const std::vector<Node> burst;
	};

	using Key = uint32_t;

	struct Data {
		const Key key;
		const std::string_view name;
		const Stats::CharacterBase baseStats;
		const std::unordered_map<Talent, std::vector<std::vector<float>>> multipliers;
		const Nodes nodes;
		const std::function<void(Character::Instance &character)> collectModifiers;
	};
}// namespace Character
