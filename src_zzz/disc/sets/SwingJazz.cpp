#include "SwingJazz.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::swingJazz{
	.key{31600},
	.goodKey{"SwingJazz"},
	.name = "Swing Jazz",
	.setup = []() -> Set::Setup {
		auto cond = IsActive("swingJazzCond");
		auto buff = Requires{
			.requirement = cond,
			.ret = Constant{.value = 0.15f},
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.er_ = Constant{.value = 0.2f},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "swingJazzCond",
						.name = "After launching a Chain Attack or Ultimate",
						.mods{
							.combat{
								.all{
									.DMG = buff,
								},
							},
						},
					},
				},
				.mods{
					.combat{
						.all{
							.DMG = buff,
						},
					},
				},
			},
		};
	},
};
