#pragma once

#include "disc/key.hpp"
#include "disc/slot.hpp"
#include "functional"
#include "option/option.hpp"
#include "setBonus.hpp"

namespace Disc {
	struct Set {
		struct Setup {
			SetBonus twoPc;
			SetBonus fourPc;

			[[nodiscard]] const SetBonus &fromSetSlot(SetSlot slot) const {
				switch (slot) {
					case Disc::SetSlot::twoPiece:
						return twoPc;
					case Disc::SetSlot::fourPiece:
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

		void getOptions(Option::TypesMap &options) const {
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
}// namespace Disc
