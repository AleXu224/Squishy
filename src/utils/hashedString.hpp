#pragma once

#include "string_view"
#include "cstdint"


namespace Utils {
	struct HashedString {
		std::string_view str;
		uint32_t hash;

		consteval HashedString(std::string_view string) : str(string), hash(fnv1a_32(string.data(), string.size())) {}
		constexpr HashedString(const char *string) : str(string), hash(fnv1a_32(str.data(), str.size())) {}

		constexpr operator uint32_t() const {
			return hash;
		}

	private:
		constexpr std::uint32_t fnv1a_32(char const *s, std::size_t count) {
			return count ? (fnv1a_32(s, count - 1) ^ s[count - 1]) * 16777619u : 2166136261u;
		}
	};
}// namespace Utils