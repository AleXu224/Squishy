#pragma once

#include "cereal/cereal.hpp"
#include "cereal/types/optional.hpp"// IWYU pragma: export
#include "cereal/types/variant.hpp" // IWYU pragma: export
#include <optional>
#include <variant>

namespace Serialization::Save {
	struct BooleanOption {
		uint32_t hash;
		bool active;

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(hash),
				CEREAL_NVP(active)
			);
		}
	};

	struct ValueListOption {
		uint32_t hash{};
		std::optional<size_t> index;

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(hash),
				CEREAL_NVP(index)
			);
		}
	};

	using OptionTypes = std::variant<Serialization::Save::BooleanOption, Serialization::Save::ValueListOption>;
}// namespace Serialization::Save