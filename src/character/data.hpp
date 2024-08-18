#pragma once

#include "conditional/conditional.hpp"
#include "key.hpp"
#include "node/node.hpp"
#include "stats/character.hpp"
#include "stats/characterBase.hpp"
#include "string_view"



namespace Character {
	struct Instance;

	struct Data {
		struct CondsSetup {
			Stats::Character &stats;
		};

		struct ModsSetup {
			Stats::Character &stats;
			const Conditional::CharacterMap &conditionals;
		};

		struct NodeSetup {
		};

		const Key key;
		const std::string_view name;
		const Stats::CharacterBase baseStats;
		const std::function<Conditional::CharacterList(Character::Data::CondsSetup data)> condsSetup;
		const std::function<void(Character::Data::ModsSetup data)> modsSetup;
		const std::function<Node::CharacterList(Character::Data::NodeSetup data)> nodeSetup;

		mutable Node::CharacterList nodes{};

		void getConds(Conditional::CharacterMap &conditionals, Stats::Character &stats) const {
			Conditional::mapConditionals(
				conditionals,
				condsSetup(CondsSetup{
					.stats = stats,
				})
			);
		}
		void getMods(Conditional::CharacterMap &conditionals, Stats::Character &stats) const {
			modsSetup(ModsSetup{
				.stats = stats,
				.conditionals = conditionals,
			});
		}
		[[nodiscard]] Node::CharacterList getNodes() const {
			return nodeSetup(NodeSetup{
			});
		}
	};
}// namespace Character

#include "formula/formula.hpp"// IWYU pragma: export
