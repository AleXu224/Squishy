#pragma once

#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "functional"
#include "option/option.hpp"
#include "setBonus.hpp"

namespace Artifact {
	struct Set {
		struct Setup {
			SetBonus twoPc;
			SetBonus fourPc;

			[[nodiscard]] const SetBonus &fromSetSlot(SetSlot slot) const {
				switch (slot) {
					case Artifact::SetSlot::twoPiece:
						return twoPc;
					case Artifact::SetSlot::fourPiece:
						return fourPc;
				}

				std::unreachable();
			}
		};

		SetKey key{};
		std::string_view goodKey;
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
