#pragma once

#include "artifact/set.hpp"         // IWYU pragma: export
#include "formula/formula.hpp"      // IWYU pragma: export
#include "modifiers/total/total.hpp"// IWYU pragma: export
#include "node/atkNode.hpp"         // IWYU pragma: export
#include "node/infoNode.hpp"        // IWYU pragma: export


namespace Artifact {
	using IsActive = Formula::impl_IsActive<Stats::ArtifactSheet>;
	using GetFloat = Formula::impl_GetFloat<Stats::ArtifactSheet>;

	using Modifiers::total;
	using Modifiers::totalEnemy;
	using Modifiers::totalTalents;
};// namespace Artifact
using namespace Formula;
