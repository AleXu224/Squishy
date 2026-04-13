#pragma once

#include "character/instance.hpp"
#include "formula/base.hpp"
#include "solution.hpp"
#include "team/instance.hpp"


namespace Optimization {
	struct OptimizationThreadData {
		Character::Instance character;
		std::array<std::optional<Character::Instance>, 4> teamCharacters;
		Team::Instance team;
		Formula::Context ctx;

		OptimizationThreadData(Character::Instance &character, const Formula::Context &ctx);
	};

	struct Optimization {
		Character::Instance &character;
		Formula::Context ctx;
		Formula::FloatNode optimizedNode;

		const ::Optimization::Options &options;

		Solutions optimize() const;
	};
}// namespace Optimization