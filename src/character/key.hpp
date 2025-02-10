#pragma once

#include "cstdint"
#include "functional"
#include "utils/key.hpp"

namespace Character {
	struct InstanceKey : public Utils::GenericKey<InstanceKey> {
		using Utils::GenericKey<InstanceKey>::GenericKey;
	};
	struct DataKey : public Utils::GenericKey<DataKey> {
		using Utils::GenericKey<DataKey>::GenericKey;
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