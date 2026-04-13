#pragma once

#include "cstdint"
#include "functional"
#include "utils/key.hpp"

namespace Agent {
	struct InstanceKey : public Utils::GenericKey<InstanceKey> {
		using Utils::GenericKey<InstanceKey>::GenericKey;
	};
	struct DataKey : public Utils::GenericKey<DataKey> {
		using Utils::GenericKey<DataKey>::GenericKey;
	};
}// namespace Agent

template<>
struct std::hash<Agent::InstanceKey> {
	std::size_t operator()(const Agent::InstanceKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};
template<>
struct std::hash<Agent::DataKey> {
	std::size_t operator()(const Agent::DataKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};