#pragma once

#include "cereal/cereal.hpp"
#include "cstdint"
#include "functional"


namespace Weapon {
	struct InstanceKey {
		uint32_t key;

		bool operator==(const InstanceKey &other) const {
			return key == other.key;
		}

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(key)
			);
		}
	};
	struct DataKey {
		uint32_t key;

		bool operator==(const DataKey &other) const {
			return key == other.key;
		}

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(key)
			);
		}
	};
}// namespace Weapon

template<>
struct std::hash<Weapon::InstanceKey> {
	std::size_t operator()(const Weapon::InstanceKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};
template<>
struct std::hash<Weapon::DataKey> {
	std::size_t operator()(const Weapon::DataKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};