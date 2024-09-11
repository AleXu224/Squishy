#pragma once

#include "cereal/cereal.hpp"
#include "cstdint"
#include "functional"


namespace Team {
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
}// namespace Team

template<>
struct std::hash<Team::InstanceKey> {
	std::size_t operator()(const Team::InstanceKey &s) const noexcept {
		return std::hash<uint32_t>{}(s.key);
	}
};