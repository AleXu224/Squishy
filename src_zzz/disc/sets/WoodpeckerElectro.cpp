#include "WoodpeckerElectro.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::woodpeckerElectro{
	.key{31000},
	.goodKey{"WoodpeckerElectro"},
	.name = "Woodpecker Electro",
	.setup = []() -> Set::Setup {
		return Set::Setup{
			.twoPc{
				.mods{
					.initial{},
				},
			},
			.fourPc{
				.opts{},
				.mods{},
			},
		};
	},
};
