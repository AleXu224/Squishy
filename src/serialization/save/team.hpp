#pragma once

#include "cereal/cereal.hpp"
#include "cereal/types/array.hpp"   // IWYU pragma: export
#include "cereal/types/optional.hpp"// IWYU pragma: export
#include "cereal/types/string.hpp"  // IWYU pragma: export


#include "character/key.hpp"
#include "team/key.hpp"

#include "array"
#include "optional"
#include "string"

namespace Serialization::Save {
	struct Team {
		::Team::InstanceKey instanceKey;
		std::string name;
		std::array<std::optional<::Character::InstanceKey>, 4> characters;

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(instanceKey),
				CEREAL_NVP(name),
				CEREAL_NVP(characters)
			);
		}
	};
}// namespace Serialization::Save