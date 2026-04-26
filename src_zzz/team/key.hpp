#pragma once

#include "cstdint"
#include "functional"
#include "utils/key.hpp"


namespace Team {
	struct InstanceKey : public Utils::GenericKey<InstanceKey> {
		using Utils::GenericKey<InstanceKey>::GenericKey;
	};
}// namespace Team

template<>
struct std::hash<Team::InstanceKey> {
	std::size_t operator()(const Team::InstanceKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};