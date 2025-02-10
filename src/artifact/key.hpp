#pragma once

#include "cstdint"
#include "functional"
#include "utils/key.hpp"


namespace Artifact {
	struct InstanceKey : public Utils::GenericKey<InstanceKey> {
		using Utils::GenericKey<InstanceKey>::GenericKey;
	};
	struct SetKey : public Utils::GenericKey<SetKey> {
		using Utils::GenericKey<SetKey>::GenericKey;
	};
}// namespace Artifact

template<>
struct std::hash<Artifact::InstanceKey> {
	std::size_t operator()(const Artifact::InstanceKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};
template<>
struct std::hash<Artifact::SetKey> {
	std::size_t operator()(const Artifact::SetKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};