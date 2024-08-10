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
		struct CondsSetup {
			Stats::Artifact &stats;
		};

		struct ModsSetup {
			Stats::Artifact &stats;
		};

		SetKey key{};
		std::string_view name;

		std::function<Conditional::ArtifactList(Artifact::Set::CondsSetup data)> condsSetup;
		std::function<void(Artifact::Set::ModsSetup data)> twoPcModsSetup;
		std::function<void(Artifact::Set::ModsSetup data)> fourPcModsSetup;
		std::function<Node::ArtifactList()> nodeSetup;

		mutable Node::ArtifactList nodes{};

		void getConds(Conditional::ArtifactMap &conditionals, Stats::Artifact &stats) const {
			Conditional::mapConditionals(
				conditionals,
				condsSetup(CondsSetup{
					.stats = stats,
				})
			);
		}
		void getModsTwo(Stats::Artifact &stats) const {
			twoPcModsSetup(ModsSetup{
				.stats = stats,
			});
		}
		void getModsFour(Stats::Artifact &stats) const {
			fourPcModsSetup(ModsSetup{
				.stats = stats,
			});
		}
		[[nodiscard]] Node::ArtifactList getNodes() const {
			return nodeSetup();
		}
	};
}// namespace Artifact
