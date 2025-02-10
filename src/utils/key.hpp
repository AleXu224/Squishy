#pragma once

#include "cereal/cereal.hpp"
#include "cstdint"

namespace Utils {
	template<class T>// Templated to make it a unique type of key, don't want to mix keys
	struct GenericKey {
		uint32_t key;

		GenericKey() : key(0) {}
		GenericKey(uint32_t key) : key(key) {}

		bool operator==(const GenericKey &other) const {
			return key == other.key;
		}

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(key)
			);
		}

		void clear() {
			key = 0;
		}

		[[nodiscard]] operator bool() const {
			return key != 0;
		}
	};
}// namespace Utils
