#include "BranchBladeSong.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::branchBladeSong{
	.key{32700},
	.goodKey{"BranchBladeSong"},
	.name = "Branch & Blade Song",
	.setup = []() -> Set::Setup {
		auto buff = Requires{
			.requirement = combat.am > 115.f,
			.ret = Constant{.value = 0.3f},
		};

		auto cond = IsActive("branchBladeSongCond");
		auto buff2 = Requires{
			.requirement = cond,
			.ret = Constant{.value = 0.12f},
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.cd = Constant{.value = 0.16f},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "branchBladeSongCond",
						.name = "Any squad member applies Freeze or triggers the Shatter effect on an enemy",
						.mods{
							.combat{
								.cr = buff2,
							},
						},
					},
				},
				.mods{
					.combat{
						.cr = buff2,
						.cd = buff,
					},
				},
			},
		};
	},
};
