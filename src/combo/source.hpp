#pragma once

#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "character/key.hpp"
#include "combo/key.hpp"
#include "misc/reaction.hpp"
#include "node/entry.hpp"
#include "node/node.hpp"
#include "variant"
#include "weapon/key.hpp"


namespace Combo::Source {
	struct Character {
		::Character::DataKey key;
		Node::CharacterSlot slot;
		size_t index;

		[[nodiscard]] Node::Instance resolve(const std::vector<::Combo::Option> &options) const;
	};

	struct Combo {
		::Character::InstanceKey characterKey;
		::Combo::InstanceKey comboKey;

		[[nodiscard]] Node::Instance resolve(const std::vector<::Combo::Option> &options) const;
	};

	struct Weapon {
		::Weapon::DataKey key;
		size_t index;

		[[nodiscard]] Node::Instance resolve(const std::vector<::Combo::Option> &options) const;
	};

	struct Artifact {
		::Artifact::SetKey key;
		::Artifact::SetSlot slot;
		size_t index;

		[[nodiscard]] Node::Instance resolve(const std::vector<::Combo::Option> &options) const;
	};

	struct TransformativeReaction {
		Misc::TransformativeReaction reaction;

		[[nodiscard]] Node::Instance resolve(const std::vector<::Combo::Option> &options) const;
	};

	using Types = std::variant<Character, Combo, Weapon, Artifact, TransformativeReaction>;
}// namespace Combo::Source