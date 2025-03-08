#pragma once

#include "character/key.hpp"
#include "team/instance.hpp"
#include "team/key.hpp"


#include "array"
#include "optional"
#include "string"

namespace Serialization::Save {
	struct Team {
		::Team::InstanceKey instanceKey;
		std::string name;
		std::array<std::optional<::Character::InstanceKey>, 4> characters;

		static Team fromInstance(const ::Team::Instance &);
		::Team::Instance toInstance() const;
	};
}// namespace Serialization::Save