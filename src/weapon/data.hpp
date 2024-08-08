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
			Stats::Weapon &stats;
			const Multipliers &multipliers;
		};

		struct ModsSetup {
			Stats::Weapon &stats;
			const Multipliers &multipliers;
			const Conditional::WeaponMap &conditionals;
		};

		struct NodeSetup {
			const Multipliers &multipliers;
		};

		const Key key;
		const std::string name;
		const Stats::WeaponBase baseStats;
		const Multipliers multipliers;
		const std::function<Conditional::WeaponList(Weapon::Data::CondsSetup data)> condsSetup;
		const std::function<void(Weapon::Data::ModsSetup data)> modsSetup;
		const std::function<Node::WeaponList(Weapon::Data::NodeSetup data)> nodeSetup;

		mutable Node::WeaponList nodes{};

		void getConds(Conditional::WeaponMap &conditionals, Stats::Weapon &stats) const {
			Conditional::mapConditionals(
				conditionals,
				condsSetup(CondsSetup{
					.stats = stats,
					.multipliers = multipliers,
				})
			);
		}
		void getMods(Conditional::WeaponMap &conditionals, Stats::Weapon &stats) const {
			modsSetup(ModsSetup{
				.stats = stats,
				.multipliers = multipliers,
				.conditionals = conditionals,
			});
		}
		[[nodiscard]] Node::WeaponList getNodes() const {
			return nodeSetup(NodeSetup{
				.multipliers = multipliers,
			});
		}
	};
}// namespace Weapon
