#pragma once

#include "key.hpp"
#include "node/node.hpp"
#include "option/option.hpp"
#include "stats/agent.hpp"
#include "stats/agentBase.hpp"
#include "string_view"


namespace Agent {
	struct Data {
		struct Setup {
			Stats::ModsSheet mods{};
			Option::AgentList opts{};
			Node::AgentList nodes{};
		};

		DataKey key;
		std::string_view goodKey;
		std::string_view name;
		Stats::AgentBase baseStats;
		std::function<Setup(void)> setup;

		std::unique_ptr<Setup> data = [](const std::function<Setup(void)> &setup) {
			return std::make_unique<Setup>(setup());
		}(setup);

		void getOpts(Option::TypesMap &options) const {
			Option::mapOptions(
				options,
				data->opts
			);
		}
	};
}// namespace Agent
