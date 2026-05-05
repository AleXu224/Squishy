#pragma once

#include "agent/key.hpp"
#include "option.hpp"
#include "team/instance.hpp"
#include "team/key.hpp"


#include "array"
#include "optional"
#include "string"

namespace Serialization::Save {
	struct Team {
		::Team::InstanceKey instanceKey;
		std::string name;
		std::array<std::optional<::Agent::InstanceKey>, 3> agents;
		uint32_t activeAgentIndex;
		std::vector<Serialization::Save::OptionTypes> options;

		static Team fromInstance(const ::Team::Instance &);
		::Team::Instance toInstance() const;
	};
}// namespace Serialization::Save