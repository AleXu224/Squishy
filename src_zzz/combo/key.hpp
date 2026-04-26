#pragma once

#include "cstdint"
#include "functional"
#include "utils/key.hpp"

namespace Combo {
	struct InstanceKey : public Utils::GenericKey<InstanceKey> {
		using Utils::GenericKey<InstanceKey>::GenericKey;
	};
}// namespace Combo

template<>
struct std::hash<Combo::InstanceKey> {
	std::size_t operator()(const Combo::InstanceKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};
