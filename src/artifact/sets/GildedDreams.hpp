#pragma once

#include "artifact/setup.hpp"

namespace Artifact::Sets {
	const inline Artifact::Set gildedDreams{
		.key = 15026,
		.name = "Gilded Dreams",
		.opts{
			// FIXME: this should be done automagically
			Option::ValueList{
				.key = "gildedSameElement",
				.prefix = "Characters with the same Elemental Type",
				.values = {1, 2, 3},
			},
			Option::ValueList{
				.key = "gildedOtherElement",
				.prefix = "Characters with a different Elemental Type",
				.values = {1, 2, 3},
			},
		},
		.setup = []() -> Set::Setup {
			return Set::Setup{
				.twoPcMods{
					.preMod{
						.em = ConstantFlat(80.f),
					},
				},
				.fourPcMods{
					.preMod{
						.atk_ = 0.14f * GetFloat("gildedSameElement"),
						.em = ConstantFlat(50.f) * GetFloat("gildedOtherElement"),
					},
				},
			};
		},
	};
}// namespace Artifact::Sets
