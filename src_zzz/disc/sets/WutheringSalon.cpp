#include "WutheringSalon.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::wutheringSalon{
	.key{33900},
	.goodKey{"WutheringSalon"},
	.name = "Wuthering Salon",
	.setup = []() -> Set::Setup {
		auto stacks = GetFloat("wutheringSalonExStacks");
		auto apBuff = stacks * 25.f;

		auto windsweptCond = IsActive("wutheringSalonWindsweptCond");
		auto allDmgBuff = Requires{
			.requirement = windsweptCond,
			.ret = Constant{.value = 0.18f},
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.wind{
							.DMG = Constant{.value = 0.10f},
						},
					},
				},
			},
			.fourPc{
				.opts{
					Option::ValueSlider{
						.key = "wutheringSalonExStacks",
						.name = "After the equipper uses an EX Special Attack",
						.values = std::views::iota(0, 3) | std::ranges::to<std::vector<float>>(),
						.mods{
							.combat{
								.ap = apBuff,
							},
						},
					},
					Option::Boolean{
						.key = "wutheringSalonWindsweptCond",
						.name = "After the equipper triggers Windswept",
						.mods{
							.combat{
								.all{
									.DMG = allDmgBuff,
								},
							},
						},
					},
				},
				.mods{
					.combat{
						.ap = apBuff,
						.all{
							.DMG = allDmgBuff,
						},
					},
				},
			},
		};
	},
};
