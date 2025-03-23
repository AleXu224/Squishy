#include "OceanHuedClam.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::oceanHuedClam{
	.key{15022},
	.goodKey{"OceanHuedClam"},
	.name = "Ocean-Hued Clam",
	.setup = []() -> Set::Setup {
		auto fourPcDmg = Constant(0.9f) * ConstantFlat(30000.f) * EnemyResMultiplier{.element = Misc::Element::physical};

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.hb = Constant(0.15f)},
				},
			},
			.fourPc{
				.nodes{
					Node::Info{
						.name = "Sea-Dyed Foam DMG",
						.formula = fourPcDmg,
					},
				},
			},
		};
	},
};
