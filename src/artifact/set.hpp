#pragma once

#include "artifact/instance.hpp"
#include "conditional/conditional.hpp"
#include "functional"
#include "node/node.hpp"
#include "stats/sheet.hpp"

namespace Character {
	struct Instance;
}

namespace Artifact {
	struct Set {
		struct Setup {
			Stats::ModsSheet twoPcMods{};
			Stats::ModsSheet fourPcMods{};
			Node::ArtifactList nodes{};
		};

		SetKey key{};
		std::string_view name;

		Conditional::ArtifactList conds{};
		std::function<Setup(void)> setup;

		Setup data = [](const std::function<Setup(void)> &setup) {
			return setup();
		}(setup);

		void getConds(Conditional::ArtifactMap &conditionals) const {
			Conditional::mapConditionals(
				conditionals,
				conds
			);
		}
	};
}// namespace Artifact
