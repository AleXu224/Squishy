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

	struct ComboCombo {
		::Character::InstanceKey characterKey;
		::Combo::InstanceKey comboKey;
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

	struct TransformativeReactionCombo {
		Misc::TransformativeReaction reaction;
	};

	using ComboSourceTypes = std::variant<CharacterCombo, ComboCombo, WeaponCombo, ArtifactCombo, TransformativeReactionCombo>;

	struct ComboOptionBool {
		bool value;
	};

	struct ComboOptionValueList {
		std::optional<uint8_t> value;
	};

	using ComboOptionTypes = std::variant<ComboOptionBool, ComboOptionValueList>;

	struct ComboOption {
		Character::InstanceKey key;
		uint32_t hash;
		ComboOptionTypes value;
	};

	struct ComboEntry {
		float multiplier;
		Misc::NodeReaction reaction;
		ComboSourceTypes source;
		std::vector<ComboOption> options;

		bool operator==(auto &&other) const {
			return false;
		}

		bool operator<(auto &&other) const {
			return false;
		}
	};

	struct ComboStateChangeEntry {
		std::vector<ComboOption> options;

		bool operator==(auto &&other) const {
			return false;
		}

		bool operator<(auto &&other) const {
			return false;
		}
	};

	using ComboEntryTypes = std::variant<ComboEntry, ComboStateChangeEntry>;

	struct Combo {
		::Combo::InstanceKey instanceKey;
		std::string name;
		std::vector<ComboEntryTypes> entries;
	};

	std::vector<Serialization::Save::Combo> comboFromInstance(const std::unordered_map<::Combo::InstanceKey, ::Combo::Combo> &combos);
	std::unordered_map<::Combo::InstanceKey, ::Combo::Combo> comboToInstance(const std::vector<Serialization::Save::Combo> &combos);
}// namespace Serialization::Save
template<>
struct glz::meta<Serialization::Save::CharacterCombo> {
	using T = Serialization::Save::CharacterCombo;
	static constexpr auto value = object(&T::key, &T::slot, &T::index);
};
template<>
struct glz::meta<Serialization::Save::ComboCombo> {
	using T = Serialization::Save::ComboCombo;
	static constexpr auto value = object(&T::characterKey, &T::comboKey);
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
struct glz::meta<Serialization::Save::TransformativeReactionCombo> {
	using T = Serialization::Save::TransformativeReactionCombo;
	static constexpr auto value = object(&T::reaction);
};
template<>
struct glz::meta<Serialization::Save::ComboSourceTypes> {
	static constexpr std::string_view tag = "type";
};

template<>
struct glz::meta<Serialization::Save::ComboOptionBool> {
	using T = Serialization::Save::ComboOptionBool;
	static constexpr auto value = object(&T::value);
};
template<>
struct glz::meta<Serialization::Save::ComboOptionValueList> {
	using T = Serialization::Save::ComboOptionValueList;
	static constexpr auto value = object(&T::value);
};
template<>
struct glz::meta<Serialization::Save::ComboOptionTypes> {
	static constexpr std::string_view tag = "type";
};

template<>
struct glz::meta<Serialization::Save::ComboEntry> {
	using T = Serialization::Save::ComboEntry;
	static constexpr auto value = object(&T::multiplier, &T::reaction, &T::source, &T::options);
};
template<>
struct glz::meta<Serialization::Save::ComboStateChangeEntry> {
	using T = Serialization::Save::ComboStateChangeEntry;
	static constexpr auto value = object(&T::options);
};
template<>
struct glz::meta<Serialization::Save::ComboEntryTypes> {
	static constexpr std::string_view tag = "type";
};