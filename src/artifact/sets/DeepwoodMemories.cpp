#include "DeepwoodMemories.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::deepwoodMemories{
	.key{15025},
	.name = "Deepwood Memories",
	.setup = []() -> Set::Setup {
		auto enemyHit = IsActive("deepwoodEnemyHit");
		auto dendroRes = Requires(enemyHit, Constant(-0.3f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{
						.dendro{
							.DMG = Constant(0.15f),
						},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "deepwoodEnemyHit",
						.name = "Opponents hit by Elemental Skill or Burst",
						.teamBuff = true,
						.mods{
							.enemy{
								.resistance{
									.dendro = dendroRes,
								},
							},
						},
					},
				},
				.mods{
					.enemy{
						.resistance{
							.dendro = dendroRes,
						},
					},
				},
			},
		};
	},
};
