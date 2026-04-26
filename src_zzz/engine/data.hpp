#pragma once

#include "engine/key.hpp"
#include "functional"
#include "node/node.hpp"
#include "option/option.hpp"
#include "stats/engineBase.hpp"
#include "stats/sheet.hpp"


namespace Engine {
	struct Data {
		struct Setup {
			Stats::ModsSheet mods{};
			Option::EngineList opts{};
			Node::EngineList nodes{};
		};

		DataKey key;
		std::string_view goodKey;
		std::string name;
		Stats::EngineBase baseStats;
		std::function<Setup(void)> setup;

		Setup data = [](const std::function<Setup(void)> &setup) {
			return setup();
		}(setup);

		void getOpts(Option::TypesMap &options) const {
			Option::mapOptions(
				options,
				data.opts
			);
		}
	};
}// namespace Engine
