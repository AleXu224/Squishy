#pragma once

#include "artifact/set.hpp"

namespace Artifact::Sets {
	const inline Artifact::Set gildedDreams{
		.key = 15026,
		.name = "Gilded Dreams",
		.conds{
			// FIXME: this should be done automagically
			Conditional::ValueList{
				.key = "gildedSameElement",
				.prefix = "Characters with the same Elemental Type",
				.values = {1, 2, 3},
			},
			Conditional::ValueList{
				.key = "gildedOtherElement",
				.prefix = "Characters with a different Elemental Type",
				.values = {1, 2, 3},
			},
		},
		.setup = []() -> Set::Setup {
			return Set::Setup{
				.twoPcMods{
					.preMod{
						.em = Formula::ConstantFlat(80.f),
					},
				},
				.fourPcMods{
					.preMod{
						.atk_ = 0.14f * Formula::ConditionalValue(Conditional::Location::artifact, "gildedSameElement"),
						.em = Formula::ConstantFlat(50.f) * Formula::ConditionalValue(Conditional::Location::artifact, "gildedOtherElement"),
					},
				},
			};
		},
	};
}// namespace Artifact::Sets
