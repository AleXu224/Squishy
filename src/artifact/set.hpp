#pragma once

#include "functional"
#include "node/node.hpp"
#include "option/option.hpp"
#include "stats/sheet.hpp"

namespace Artifact {
	struct SetBonus {
		Option::ArtifactList opts{};
		Stats::ModsSheet mods{};
		Node::ArtifactList nodes{};
	};
	struct Set {
		struct Setup {
			SetBonus twoPc;
			SetBonus fourPc;
		};

		SetKey key{};
		std::string_view name;

		std::function<Setup(void)> setup;

		Setup data = [](const std::function<Setup(void)> &setup) {
			return setup();
		}(setup);

		void getOptions(Option::ArtifactMap &options) const {
			Option::mapOptions(
				options,
				data.twoPc.opts
			);
			Option::mapOptions(
				options,
				data.fourPc.opts
			);
		}
	};
}// namespace Artifact
