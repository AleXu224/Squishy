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
			Stats::Sheet &stats;
		};

		struct ModsSetup {
			Stats::Sheet &stats;
			const Conditional::ArtifactMap &conditionals;
		};

		struct NodeSetup {
			Stats::Sheet &stats;
			const Conditional::ArtifactMap &conditionals;
		};

		SetKey key{};
		std::string_view name;

		const std::function<Conditional::ArtifactList(Artifact::Set::CondsSetup data)> condsSetup;
		const std::function<void(Artifact::Set::ModsSetup data)> twoPcModsSetup;
		const std::function<void(Artifact::Set::ModsSetup data)> fourPcModsSetup;
		const std::function<Node::List(Artifact::Set::NodeSetup data)> nodeSetup;

		void getConds(Conditional::ArtifactMap &conditionals, Stats::Sheet &stats) const {
			Conditional::mapConditionals(
				conditionals,
				condsSetup(CondsSetup{
					.stats = stats,
				})
			);
		}
		void getModsTwo(Conditional::ArtifactMap &conditionals, Stats::Sheet &stats) const {
			twoPcModsSetup(ModsSetup{
				.stats = stats,
				.conditionals = conditionals,
			});
		}
		void getModsFour(Conditional::ArtifactMap &conditionals, Stats::Sheet &stats) const {
			fourPcModsSetup(ModsSetup{
				.stats = stats,
				.conditionals = conditionals,
			});
		}
		void getNodes(Conditional::ArtifactMap &conditionals, Node::List &nodes, Stats::Sheet &stats) const {
			Node::List::combineNodes(
				nodes,
				nodeSetup(NodeSetup{
					.stats = stats,
					.conditionals = conditionals,
				})
			);
		}
	};
}// namespace Artifact
