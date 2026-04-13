#include "AubadeOfMorningstarAndMoon.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::aubadeOfMorningstarAndMoon{
	.key{15043},
	.goodKey{"AubadeOfMorningstarAndMoon"},
	.name = "Aubade of Morningstar and Moon",
	.setup = []() -> Set::Setup {
		auto cond = IsActive("aubade4pcOffField");
		auto buff = Requires{
			.requirement = cond,
			.ret = Constant{.value = 0.2f}
				 + Requires{
					 .requirement = Requirement::ascendantGleam,
					 .ret = Constant{.value = 0.4f},
				 },
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{
						.em = ConstantFlat{.value = 80.f},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "aubade4pcOffField",
						.name = "Character is Off-Field",
						.mods{
							.preMod{
								.allLunar{
									.DMG = buff,
								},
							},
						},
					},
				},
				.mods{
					.preMod{
						.allLunar{
							.DMG = buff,
						},
					},
				},
			},
		};
	},
};
