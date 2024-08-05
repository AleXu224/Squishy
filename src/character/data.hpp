#pragma once

#include "conditional/conditional.hpp"
#include "key.hpp"
#include "node/node.hpp"
#include "stats/characterBase.hpp"
#include "string_view"


namespace Character {
	struct Instance;

	struct Multipliers {
		std::vector<std::array<float, 15>> normal;
		std::vector<std::array<float, 15>> skill;
		std::vector<std::array<float, 15>> burst;
		std::vector<std::array<float, 15>> passive1{};
		std::vector<std::array<float, 15>> passive2{};
		std::vector<std::array<float, 15>> constellation1{};
		std::vector<std::array<float, 15>> constellation2{};
		std::vector<std::array<float, 15>> constellation4{};
		std::vector<std::array<float, 15>> constellation6{};
	};

	struct Data {
		struct NodeSetup {
			Stats::Character &stats;
			const Multipliers &multipliers;
		};

		struct ModsSetup {
			Stats::Character &stats;
			const Multipliers &multipliers;
		};

		struct CondsSetup {
			Stats::Character &stats;
			const Multipliers &multipliers;
		};

		const Key key;
		const std::string_view name;
		const Stats::CharacterBase baseStats;
		const Multipliers multipliers;
		const std::function<Conditional::List(Character::Data::CondsSetup data)> condsSetup;
		const std::function<void(Character::Data::ModsSetup data)> modsSetup;
		const std::function<Node::List(Character::Data::NodeSetup data)> nodeSetup;

		[[nodiscard]] Conditional::ListMapped getConds(Stats::Character &stats) const {
			return Conditional::mapConditionals(
				condsSetup(CondsSetup{
					.stats = stats,
					.multipliers = multipliers,
				})
			);
		}
		void getMods(Stats::Character &stats) const {
			modsSetup(ModsSetup{
				.stats = stats,
				.multipliers = multipliers,
			});
		}
		[[nodiscard]] Node::List getNodes(Stats::Character &stats) const {
			return nodeSetup(NodeSetup{
				.stats = stats,
				.multipliers = multipliers,
			});
		}
	};
}// namespace Character
