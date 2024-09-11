#pragma once

#include "cereal/cereal.hpp"
#include "cstdint"
#include "functional"


namespace Artifact {
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
	struct SetKey {
		uint32_t key;

		bool operator==(const SetKey &other) const {
			return key == other.key;
		}

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(key)
			);
		}
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