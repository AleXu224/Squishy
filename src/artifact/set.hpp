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

		mutable std::unique_ptr<const Setup> data = nullptr;

		void init() const {
			data = std::make_unique<Setup>(setup());
		}

		void getOptions(Option::TypesMap &options) const {
			Option::mapOptions(
				options,
				data->twoPc.opts
			);
			Option::mapOptions(
				options,
				data->fourPc.opts
			);
		}
	};
}// namespace Artifact
