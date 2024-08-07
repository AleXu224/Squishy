#pragma once

#include "conditional/conditional.hpp"
#include "functional"
#include "node/node.hpp"
#include "stats/weaponBase.hpp"
#include "weapon/key.hpp"

namespace Weapon {
	using Multipliers = std::vector<std::array<float, 5>>;
	struct Data {
		struct CondsSetup {
			Stats::Sheet &stats;
			const Multipliers &multipliers;
		};

		struct ModsSetup {
			Stats::Sheet &stats;
			const Multipliers &multipliers;
			const Conditional::WeaponMap &conditionals;
		};

		struct NodeSetup {
			Stats::Sheet &stats;
			const Multipliers &multipliers;
			const Conditional::WeaponMap &conditionals;
		};

		const Key key;
		const std::string name;
		const Stats::WeaponBase baseStats;
		const Multipliers multipliers;
		const std::function<Conditional::WeaponList(Weapon::Data::CondsSetup data)> condsSetup;
		const std::function<void(Weapon::Data::ModsSetup data)> modsSetup;
		const std::function<Node::List(Weapon::Data::NodeSetup data)> nodeSetup;

		void getConds(Conditional::WeaponMap &conditionals, Stats::Sheet &stats) const {
			Conditional::mapConditionals(
				conditionals,
				condsSetup(CondsSetup{
					.stats = stats,
					.multipliers = multipliers,
				})
			);
		}
		void getMods(Conditional::WeaponMap &conditionals, Stats::Sheet &stats) const {
			modsSetup(ModsSetup{
				.stats = stats,
				.multipliers = multipliers,
				.conditionals = conditionals,
			});
		}
		void getNodes(Conditional::WeaponMap &conditionals, Node::List &nodes, Stats::Sheet &stats) const {
			Node::List::combineNodes(
				nodes,
				nodeSetup(NodeSetup{
					.stats = stats,
					.multipliers = multipliers,
					.conditionals = conditionals,
				})
			);
		}
	};
}// namespace Weapon
