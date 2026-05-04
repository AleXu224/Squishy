#include "FangedMetal.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::fangedMetal{
	.key{32600},
	.goodKey{"FangedMetal"},
	.name = "Fanged Metal",
	.setup = []() -> Set::Setup {
		auto cond = IsActive("fangedMetalCond");
		auto buff = Requires{
			.requirement = cond,
			.ret = Constant{.value = 0.35f},
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.physical{.DMG = Constant{.value = 0.1f}},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "fangedMetalCond",
						.name = "A squad member inflicted Assault on an enemy",
						.mods{
							.combat{
								.all{.DMG = buff},
							},
						},
					},
				},
				.mods{
					.combat{
						.all{.DMG = buff},
					},
				},
			},
		};
	},
};
