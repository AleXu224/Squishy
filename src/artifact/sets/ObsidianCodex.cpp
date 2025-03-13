#include "ObsidianCodex.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::obsidianCodex{
	.key{15038},
	.goodKey{"ObsidianCodex"},
	.name = "Obsidian Codex",
	.setup = []() -> Set::Setup {
		auto twoPcDMG = Requires(IsActive("obsidianCodexNightsoul"), Constant(0.15f));
		auto fourPcCr = Requires(IsActive("obsidianCodexConsumed"), Constant(0.4f));

		return Set::Setup{
			.twoPc{
				.opts{
					Option::Boolean{
						.key = "obsidianCodexNightsoul",
						.name = "Equipping character is in Nightsoul's Blessing and is on the field",
						.mods{
							.preMod{
								.all{
									.DMG = twoPcDMG,
								},
							},
						},
					},
				},
				.mods{
					.preMod{
						.all{
							.DMG = twoPcDMG
						},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "obsidianCodexConsumed",
						.name = "After the equipping character consumes 1 Nightsoul point while on the field",
						.mods{
							.preMod{
								.cr = fourPcCr,
							},
						},
					},
				},
				.mods{
					.preMod{
						.cr = fourPcCr,
					},
				},
			},
		};
	},
};
