#pragma once

#include "artifact/instance.hpp"
#include "conditional/conditional.hpp"
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
			const Conditional::ArtifactMap &conditionals;
		};

		SetKey key{};
		std::string_view name;

		const std::function<Conditional::ArtifactList(Artifact::Set::CondsSetup data)> condsSetup;
		const std::function<void(Artifact::Set::ModsSetup data)> twoPcModsSetup;
		const std::function<void(Artifact::Set::ModsSetup data)> fourPcModsSetup;
		const std::function<Node::ArtifactList()> nodeSetup;

		mutable Node::ArtifactList nodes{};

		void getConds(Conditional::ArtifactMap &conditionals, Stats::Artifact &stats) const {
			Conditional::mapConditionals(
				conditionals,
				condsSetup(CondsSetup{
					.stats = stats,
				})
			);
		}
		void getModsTwo(Conditional::ArtifactMap &conditionals, Stats::Artifact &stats) const {
			twoPcModsSetup(ModsSetup{
				.stats = stats,
				.conditionals = conditionals,
			});
		}
		void getModsFour(Conditional::ArtifactMap &conditionals, Stats::Artifact &stats) const {
			fourPcModsSetup(ModsSetup{
				.stats = stats,
				.conditionals = conditionals,
			});
		}
		[[nodiscard]] Node::ArtifactList getNodes() const {
			return nodeSetup();
		}
	};
}// namespace Artifact
