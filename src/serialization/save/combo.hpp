#pragma once

#include "misc/reaction.hpp"
#include "variant"


#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "character/key.hpp"
#include "combo/combo.hpp"
#include "node/entry.hpp"
#include "weapon/key.hpp"

#include "glaze/glaze.hpp"// IWYU pragma: keep


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

	std::vector<Serialization::Save::Combo> comboFromInstance(const std::list<::Combo::Combo> &combos);
	std::list<::Combo::Combo> comboToInstance(const std::vector<Serialization::Save::Combo> &combos);
}// namespace Serialization::Save
template<>
struct glz::meta<Serialization::Save::CharacterCombo> {
	using T = Serialization::Save::CharacterCombo;
	static constexpr auto value = object(&T::key, &T::slot, &T::index);
};
template<>
struct glz::meta<Serialization::Save::WeaponCombo> {
	using T = Serialization::Save::WeaponCombo;
	static constexpr auto value = object(&T::key, &T::index);
};
template<>
struct glz::meta<Serialization::Save::ArtifactCombo> {
	using T = Serialization::Save::ArtifactCombo;
	static constexpr auto value = object(&T::key, &T::slot, &T::index);
};
template<>
struct glz::meta<Serialization::Save::ComboSourceTypes> {
	static constexpr std::string_view tag = "type";
};