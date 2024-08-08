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
		struct CondsSetup {
			Stats::Character &stats;
			const Multipliers &multipliers;
		};

		struct ModsSetup {
			Stats::Character &stats;
			const Multipliers &multipliers;
			const Conditional::CharacterMap &conditionals;
		};

		struct NodeSetup {
			const Multipliers &multipliers;
		};

		const Key key;
		const std::string_view name;
		const Stats::CharacterBase baseStats;
		const Multipliers multipliers;
		const std::function<Conditional::CharacterList(Character::Data::CondsSetup data)> condsSetup;
		const std::function<void(Character::Data::ModsSetup data)> modsSetup;
		const std::function<Node::CharacterList(Character::Data::NodeSetup data)> nodeSetup;

		mutable Node::CharacterList nodes{};

		void getConds(Conditional::CharacterMap &conditionals, Stats::Character &stats) const {
			Conditional::mapConditionals(
				conditionals,
				condsSetup(CondsSetup{
					.stats = stats,
					.multipliers = multipliers,
				})
			);
		}
		void getMods(Conditional::CharacterMap &conditionals, Stats::Character &stats) const {
			modsSetup(ModsSetup{
				.stats = stats,
				.multipliers = multipliers,
				.conditionals = conditionals,
			});
		}
		[[nodiscard]] Node::CharacterList getNodes() const {
			return nodeSetup(NodeSetup{
				.multipliers = multipliers,
			});
		}
	};
}// namespace Character
