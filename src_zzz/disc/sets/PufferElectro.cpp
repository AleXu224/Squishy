#include "PufferElectro.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::pufferElectro{
	.key{31100},
	.goodKey{"PufferElectro"},
	.name = "Puffer Electro",
	.setup = []() -> Set::Setup {
		auto cond = IsActive("pufferElectroCond");
		auto buff = Requires{
			.requirement = cond,
			.ret = Constant{.value = 0.15f},
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.penRatio = Constant{.value = 0.08f},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "pufferElectroCond",
						.name = "Afterl launching an Ultimate",
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
						.ultimate{
							.DMG = Constant{.value = 0.2f},
						},
					},
				},
			},
		};
	},
};
