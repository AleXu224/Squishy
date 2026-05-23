#include "PolarMetal.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::polarMetal{
	.key{32500},
	.goodKey{"PolarMetal"},
	.name = "Polar Metal",
	.setup = []() -> Set::Setup {
		auto cond = IsActive("polarMetalCond");
		auto buff = Requires{
			.requirement = cond,
			.ret = Constant{.value = 0.2f},
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.ice{
							.DMG = Constant{.value = 0.10f},
						},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "polarMetalCond",
						.name = "Affter any squad member inflicts Freeze or Shatter",
						.mods{
							.combat{
								.basic{
									.DMG = buff,
								},
								.dashAttack{
									.DMG = buff,
								},
							},
						},
					},
				},
				.mods{
					.combat{
						.basic{
							.DMG = Constant{.value = 0.2f} + buff,
						},
						.dashAttack{
							.DMG = Constant{.value = 0.2f} + buff,
						},
					},
				},
			},
		};
	},
};
