#pragma once

#include "artifact/instance.hpp"
#include "conditional/conditional.hpp"
#include "formula/formula.hpp"// IWYU pragma: export
#include "functional"
#include "node/node.hpp"


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

		void getConds(Conditional::ArtifactMap &conditionals, Stats::Artifact &stats) const {
			Conditional::mapConditionals(
				conditionals,
				conds
			);
		}
	};
}// namespace Artifact
