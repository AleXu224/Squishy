#pragma once

#include "cstdint"
#include "functional"
#include "utils/key.hpp"


namespace Engine {
	struct InstanceKey : public Utils::GenericKey<InstanceKey> {
		using Utils::GenericKey<InstanceKey>::GenericKey;
	};
	struct DataKey : public Utils::GenericKey<DataKey> {
		using Utils::GenericKey<DataKey>::GenericKey;
	};
}// namespace Engine

template<>
struct std::hash<Engine::InstanceKey> {
	std::size_t operator()(const Engine::InstanceKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};
template<>
struct std::hash<Engine::DataKey> {
	std::size_t operator()(const Engine::DataKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};