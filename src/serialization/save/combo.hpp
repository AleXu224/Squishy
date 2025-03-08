#pragma once

#include "misc/reaction.hpp"
#include "variant"


#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "character/key.hpp"
#include "node/entry.hpp"
#include "weapon/key.hpp"


namespace Serialization::Save {
	struct CharacterCombo {
		::Character::DataKey key;
		Node::CharacterSlot slot;
		size_t index;
	};

	struct WeaponCombo {
		::Weapon::DataKey key;
		size_t index;
	};

	struct ArtifactCombo {
		::Artifact::SetKey key;
		::Artifact::SetSlot slot;
		size_t index;
	};

	using ComboSourceTypes = std::variant<CharacterCombo, WeaponCombo, ArtifactCombo>;

	struct ComboEntry {
		float multiplier;
		Misc::NodeReaction reaction;
		ComboSourceTypes source;
	};

	struct Combo {
		std::string name;
		std::vector<ComboEntry> entries;
	};
}// namespace Serialization::Save