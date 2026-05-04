#include "WoodpeckerElectro.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::woodpeckerElectro{
	.key{31000},
	.goodKey{"WoodpeckerElectro"},
	.name = "Woodpecker Electro",
	.setup = []() -> Set::Setup {
		auto cond = IsActive("woodpeckerElectroCond");
		auto buff = Requires{
			.requirement = cond,
			.ret = Constant{.value = 0.09},
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.cr = Constant{.value = 0.08f},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "woodpeckerElectroCond",
						.name = "After landing a critical hit on an enemy with a Basic Attack, Dodge Counter, or EX Special Attack",
						.mods{
							.combat{
								.atk_ = buff,
							},
						},
					},
				},
				.mods{
					.combat{
						.atk_ = buff,
					},
				},
			},
		};
	},
};
