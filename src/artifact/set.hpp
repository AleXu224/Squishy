#pragma once

#include "artifact/instance.hpp"
#include "functional"

namespace Character {
	struct Instance;
}

namespace Artifact {
	struct Set {
		SetKey key{};
		std::string_view name;
		const std::function<void(Character::Instance &character)> twoSet{};
		const std::function<void(Character::Instance &character)> fourSet{};
	};
}// namespace Artifact
