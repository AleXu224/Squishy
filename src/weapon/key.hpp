#pragma once

#include "cstdint"
#include "functional"
#include "utils/key.hpp"


namespace Weapon {
	struct InstanceKey : public Utils::GenericKey<InstanceKey> {
		using Utils::GenericKey<InstanceKey>::GenericKey;
	};
	struct DataKey : public Utils::GenericKey<DataKey> {
		using Utils::GenericKey<DataKey>::GenericKey;
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