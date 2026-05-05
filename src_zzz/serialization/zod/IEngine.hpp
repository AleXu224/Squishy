#pragma once

#include "../../engine/instance.hpp"
#include "cstdint"
#include "expected"
#include "string"


namespace Serialization::Zod {
	struct IEngine {
		std::string key;
		uint8_t level;
		uint8_t modification;
		uint8_t phase;
		std::string location;
		bool lock = false;

		static std::vector<IEngine> fromInstance(const Engine::Instance &engine);

		std::expected<std::reference_wrapper<Engine::Instance>, std::string> createInstance() const;

		std::expected<std::reference_wrapper<const Engine::Data>, std::string> getData() const;

		std::expected<std::reference_wrapper<Engine::Instance>, std::string> isAlreadyStored() const;

		void writeToInstance(Engine::Instance &) const;
	};
}// namespace Serialization::Zod