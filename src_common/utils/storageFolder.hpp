#pragma once

#include "filesystem"
#include "optional"

namespace Utils {
	[[nodiscard]] extern std::optional<std::filesystem::path> getStorageFolder();
}