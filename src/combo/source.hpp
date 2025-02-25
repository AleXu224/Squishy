#pragma once

#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "character/data.hpp"
#include "character/key.hpp"
#include "formula/node.hpp"
#include "node/entry.hpp"
#include "stats/loadout.hpp"
#include "variant"
#include "weapon/key.hpp"


namespace Combo::Source {
	struct Character {
		::Character::DataKey key;
		Node::CharacterSlot slot;
		size_t index;

		[[nodiscard]] const Node::Instance &resolve(const Formula::Context &context) const {
			return context.source.character.data.data.nodes.fromEntry(slot).at(index);
		}
	};

	struct Weapon {
		::Weapon::DataKey key;
		size_t index;
	};

	struct Artifact {
		::Artifact::SetKey key;
		::Artifact::SetSlot slot;
		size_t index;
	};

	using Types = std::variant<Character, Weapon, Artifact>;
}// namespace Combo::Source