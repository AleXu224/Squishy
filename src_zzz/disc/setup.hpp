#pragma once

#include "formula/base.hpp"           // IWYU pragma: export
#include "formula/multiplier.hpp"     // IWYU pragma: export
#include "formula/option.hpp"         // IWYU pragma: export
#include "modifiers/total/base.hpp"   // IWYU pragma: export
#include "modifiers/total/initial.hpp"// IWYU pragma: export
#include "modifiers/total/total.hpp"  // IWYU pragma: export
#include "node/atkNode.hpp"           // IWYU pragma: export
#include "node/infoNode.hpp"          // IWYU pragma: export
#include "set.hpp"                    // IWYU pragma: export


namespace Artifact {
	static inline auto IsActive(const Utils::HashedString &name) {
		return Formula::impl_IsActive{.name = name};
	}
	static inline auto GetFloat(const Utils::HashedString &name, float defaultValue = 0.f) {
		return Formula::impl_GetFloat{.name = name, .defaultValue = defaultValue};
	}
	static inline auto GetInt(const Utils::HashedString &name, uint32_t defaultValue = 0) {
		return Formula::impl_GetInt{.name = name, .defaultValue = defaultValue};
	}
	static inline auto GetIndex(const Utils::HashedString &name, uint32_t defaultValue = 0) {
		return Formula::impl_GetIndex{.name = name, .defaultValue = defaultValue};
	}

	// static constexpr struct Character {
	// 	static constexpr Formula::CharacterLevel level{};
	// 	static constexpr Formula::CharacterConstellation constellation{};
	// 	static constexpr Formula::CharacterAscension ascension{};
	// } character{};

	using EntryType = Utils::EntryType;

	static inline const auto &base = Modifiers::base();
	static inline const auto &initial = Modifiers::initial();
	static inline const auto &combat = Modifiers::combat();
	static inline const auto &enemy = Modifiers::enemy();
	static inline const auto &skills = Modifiers::skills();
};// namespace Artifact
using namespace Formula;
