#pragma once

#include "functional"
#include "node/node.hpp"
#include "option/option.hpp"
#include "stats/sheet.hpp"

namespace Artifact {
	struct Set {
		struct Setup {
			Stats::ModsSheet twoPcMods{};
			Stats::ModsSheet fourPcMods{};
			Node::ArtifactList nodes{};
		};

		SetKey key{};
		std::string_view name;

		Option::ArtifactList opts{};
		std::function<Setup(void)> setup;

		Setup data = [](const std::function<Setup(void)> &setup) {
			return setup();
		}(setup);

		void getOptions(Option::ArtifactMap &options) const {
			Option::mapOptions(
				options,
				opts
			);
		}
	};
}// namespace Artifact
