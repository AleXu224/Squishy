#pragma once

#include "cstdint"
#include "functional"
#include "utils/key.hpp"


namespace Disc {
	struct InstanceKey : public Utils::GenericKey<InstanceKey> {
		using Utils::GenericKey<InstanceKey>::GenericKey;
	};
	struct SetKey : public Utils::GenericKey<SetKey> {
		using Utils::GenericKey<SetKey>::GenericKey;
	};
}// namespace Disc

template<>
struct std::hash<Disc::InstanceKey> {
	std::size_t operator()(const Disc::InstanceKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};
template<>
struct std::hash<Disc::SetKey> {
	std::size_t operator()(const Disc::SetKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};