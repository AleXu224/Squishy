#pragma once

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

		void clear() {
			key = 0;
		}

		[[nodiscard]] operator bool() const {
			return key != 0;
		}

		operator uint32_t() const {
			return key;
		}

	};
	[[nodiscard]] static inline uint64_t hashCombine(uint32_t key1, uint32_t key2) {
		return (static_cast<uint64_t>(key1) << 32) | static_cast<uint64_t>(key2);
	}
}// namespace Utils
