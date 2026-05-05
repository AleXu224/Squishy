#pragma once

#include "agent/instance.hpp"
#include "cstdint"
#include "expected"
#include <string>


namespace Serialization::Zod {
	struct IAgent {
		std::string key;
		uint8_t level;
		uint8_t mindscape;
		uint8_t promotion;

		uint8_t core;
		uint8_t dodge;
		uint8_t basic;
		uint8_t chain;
		uint8_t special;
		uint8_t assist;
		uint8_t potential;

		static IAgent fromInstance(const Agent::Instance &agent);

		std::expected<std::reference_wrapper<Agent::Instance>, std::string> createInstance() const;

		std::expected<std::reference_wrapper<const Agent::Data>, std::string> getData() const;

		std::expected<std::reference_wrapper<Agent::Instance>, std::string> isAlreadyStored() const;

		void writeToInstance(Agent::Instance &) const;
	};
}// namespace Serialization::Zod
