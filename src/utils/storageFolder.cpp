#include "storageFolder.hpp"

#ifdef _WIN32
#include "ShlObj.h"// IWYU pragma: keep
#elif defined(__APPLE__)
#error "Not implemented"
#else
#include <cstdlib>
#endif

std::optional<std::filesystem::path> Utils::getStorageFolder() {
#ifdef _WIN32
	PWSTR stringPtr{};
	auto res = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DONT_VERIFY, nullptr, &stringPtr);
	if (res != S_OK) return {};

	return std::filesystem::path(stringPtr);
#elif defined(__APPLE__)
#error "Not implemented"
#else
	char *stringPtr = std::getenv("HOME");
	if (!stringPtr) return {};
	return (std::filesystem::path(stringPtr) / ".config");
#endif
}