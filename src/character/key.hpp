#pragma once

#include "cstdint"
#include "functional"

namespace Character {
	struct InstanceKey {
		uint32_t key;

		bool operator==(const InstanceKey &other) const {
			return key == other.key;
		}
	};
	struct DataKey {
		uint32_t key;

		bool operator==(const DataKey &other) const {
			return key == other.key;
		}
	};
}// namespace Character

template<>
struct std::hash<Character::InstanceKey> {
	std::size_t operator()(const Character::InstanceKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};
template<>
struct std::hash<Character::DataKey> {
	std::size_t operator()(const Character::DataKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};