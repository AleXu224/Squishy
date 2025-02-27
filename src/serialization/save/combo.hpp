#pragma once

#include "cereal/cereal.hpp"
#include "cereal/types/variant.hpp"// IWYU pragma: export
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

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(key),
				CEREAL_NVP(slot),
				CEREAL_NVP(index)
			);
		}
	};

	struct WeaponCombo {
		::Weapon::DataKey key;
		size_t index;

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(key),
				CEREAL_NVP(index)
			);
		}
	};

	struct ArtifactCombo {
		::Artifact::SetKey key;
		::Artifact::SetSlot slot;
		size_t index;

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(key),
				CEREAL_NVP(slot),
				CEREAL_NVP(index)
			);
		}
	};

	using ComboSourceTypes = std::variant<CharacterCombo, WeaponCombo, ArtifactCombo>;

	struct ComboEntry {
		float multiplier;
		Misc::NodeReaction reaction;
		ComboSourceTypes source;

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(multiplier),
				CEREAL_NVP(reaction),
				CEREAL_NVP(source)
			);
		}
	};

	struct Combo {
		std::string name;
		std::vector<ComboEntry> entries;

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(name),
				CEREAL_NVP(entries)
			);
		}
	};
}// namespace Serialization::Save