#pragma once

#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"// IWYU pragma: export
#include "option.hpp"
#include "weapon/key.hpp"

namespace Serialization::Save {
	struct Weapon {
		::Weapon::InstanceKey instanceKey;
		::Weapon::DataKey dataKey;
		uint8_t level;
		uint8_t ascension;
		uint8_t refinement;
		std::vector<Serialization::Save::OptionTypes> options;

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(instanceKey),
				CEREAL_NVP(dataKey),
				CEREAL_NVP(level),
				CEREAL_NVP(ascension),
				CEREAL_NVP(refinement),
				CEREAL_NVP(options)
			);
		}
	};
}// namespace Serialization::Save