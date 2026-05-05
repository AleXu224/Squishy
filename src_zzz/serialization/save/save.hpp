#pragma once

#include "disc.hpp"
#include "agent.hpp"
#include "team.hpp"
#include "engine.hpp"
#include "window.hpp"


namespace Serialization::Save {
	struct Save {
		std::vector<Serialization::Save::Disc> discs;
		std::vector<Serialization::Save::Engine> engines;
		std::vector<Serialization::Save::Agent> agents;
		std::vector<Serialization::Save::Team> teams;
		Serialization::Save::Window window;
	};
}// namespace Serialization::Save