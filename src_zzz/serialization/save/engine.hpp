#pragma once

#include "../../engine/instance.hpp"
#include "engine/key.hpp"


namespace Serialization::Save {
	struct Engine {
		::Engine::InstanceKey instanceKey;
		::Engine::DataKey dataKey;
		uint8_t level;
		uint8_t modification;
		uint8_t overclock;

		static Engine fromInstance(const ::Engine::Instance &);
		::Engine::Instance toInstance() const;
	};
}// namespace Serialization::Save