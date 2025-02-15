#pragma once

#include "artifact/setup.hpp"

namespace Artifact::Sets {
	const inline Artifact::Set goldenTroupe{
		.key{15032},
		.name = "Golden Troupe",
		.setup = []() -> Set::Setup {
			return Set::Setup{
				.twoPc{
					.mods{
						.preMod{
							.skill{
								.DMG = Constant(0.2f),
							},
						},
					},
				},
				.fourPc{
					.opts{
						Option::Boolean{
							.key = "goldenTroupeOffField",
							.name = "Character is off field",
						},
					},
					.mods{
						.preMod{
							.skill{
								.DMG = Constant(0.25f)
									 + Requires(
										   IsActive("goldenTroupeOffField"),
										   Constant(0.25f)
									 ),
							},
						},
					},
				},
			};
		},
	};
}// namespace Artifact::Sets
