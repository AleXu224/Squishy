#include "FreedomBlues.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::freedomBlues{
	.key{31300},
	.goodKey{"FreedomBlues"},
	.name = "Freedom Blues",
	.setup = []() -> Set::Setup {
		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.ap = ConstantFlat{.value = 30.f},
					},
				},
			},
			.fourPc{
				.opts{},
				.mods{},
			},
		};
	},
};
