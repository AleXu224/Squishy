#pragma once

#include "artifact/key.hpp"
#include "artifact/sets.hpp"
#include "artifact/slot.hpp"
#include "character/characters.hpp"
#include "character/data.hpp"
#include "character/key.hpp"
#include "node/entry.hpp"
#include "variant"
#include "weapon/data.hpp"
#include "weapon/key.hpp"
#include "weapon/weapons.hpp"


namespace Combo::Source {
	struct Character {
		::Character::DataKey key;
		Node::CharacterSlot slot;
		size_t index;

		[[nodiscard]] const Node::Instance &resolve(const Formula::Context &context) const {
			return ::Character::list.at(key).data.nodes.fromEntry(slot).at(index);
		}
	};

	struct Weapon {
		::Weapon::DataKey key;
		size_t index;

		[[nodiscard]] const Node::Instance &resolve(const Formula::Context &context) const {
			return ::Weapon::list.at(key).data.nodes.at(index);
		}
	};

	struct Artifact {
		::Artifact::SetKey key;
		::Artifact::SetSlot slot;
		size_t index;

		[[nodiscard]] const Node::Instance &resolve(const Formula::Context &context) const {
			return ::Artifact::sets.at(key).data.fromSetSlot(slot).nodes.at(index);
		}
	};

	using Types = std::variant<Character, Weapon, Artifact>;
}// namespace Combo::Source