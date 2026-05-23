#include "AstralVoice.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::astralVoice{
	.key{32800},
	.goodKey{"AstralVoice"},
	.name = "Astral Voice",
	.setup = []() -> Set::Setup {
		auto stacks = GetFloat("astralVoiceStacks");
		auto buff = stacks * 0.08f;

		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.atk_ = Constant{.value = 0.1f},
					},
				},
			},
			.fourPc{
				.opts{
					Option::ValueSlider{
						.key = "astralVoiceStacks",
						.name = "After any squad member enters the field using a Quick Assist",
						.values = std::views::iota(0, 4) | std::ranges::to<std::vector<float>>(),
						.mods{
							.activeCombat{
								.all{
									.DMG = buff,
								},
							},
						},
					},
				},
				.mods{
					.activeCombat{
						.all{
							.DMG = buff,
						},
					},
				},
			},
		};
	},
};
