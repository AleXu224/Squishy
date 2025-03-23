#pragma once

#include "artifact/set.hpp"         // IWYU pragma: export
#include "formula/formula.hpp"      // IWYU pragma: export
#include "modifiers/total/total.hpp"// IWYU pragma: export
#include "node/atkNode.hpp"         // IWYU pragma: export
#include "node/healNode.hpp"        // IWYU pragma: export
#include "node/infoNode.hpp"        // IWYU pragma: export


namespace Artifact {
	using IsActive = Formula::impl_IsActive<Stats::ArtifactSheet>;
	using GetFloat = Formula::impl_GetFloat<Stats::ArtifactSheet>;
	using GetInt = Formula::impl_GetInt<Stats::ArtifactSheet>;

	static constexpr struct Character {
		static constexpr Formula::CharacterLevel level{};
		static constexpr Formula::CharacterConstellation constellation{};
		static constexpr Formula::CharacterAscension ascension{};
	} character{};

	using EntryType = Utils::EntryType;

	using Modifiers::preMods;
	using Modifiers::total;
	using Modifiers::totalActive;
	using Modifiers::totalEnemy;
	using Modifiers::totalTalents;
};// namespace Artifact
using namespace Formula;
