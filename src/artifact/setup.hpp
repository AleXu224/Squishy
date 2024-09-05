#pragma once

#include "artifact/set.hpp"   // IWYU pragma: export
#include "formula/formula.hpp"// IWYU pragma: export
#include "node/atkNode.hpp"   // IWYU pragma: export

namespace Artifact {
	using IsActive = Formula::impl_IsActive<Stats::ArtifactSheet>;
	using GetFloat = Formula::impl_GetFloat<Stats::ArtifactSheet>;
};// namespace Artifact
using namespace Formula;
