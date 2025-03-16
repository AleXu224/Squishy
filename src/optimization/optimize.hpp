#pragma once

#include "character/instance.hpp"
#include "formula/formulaContext.hpp"
#include "solution.hpp"

namespace Optimization {
	struct Optimization {
		Character::Instance &character;
		Formula::Context ctx;
		Formula::FloatNode optimizedNode;

		bool threeRainbow = true;
		bool fiveRainbow = true;
		std::vector<Artifact::SetKey> enabledTwoPiece;
		std::vector<Artifact::SetKey> enabledFourPiece;

		Solutions optimize() const;
	};
}// namespace Optimization