#include "ViridescentVenerer.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::viridescentVenerer{
	.key{15002},
	.goodKey{"ViridescentVenerer"},
	.name = "Viridescent Venerer",
	.setup = []() -> Set::Setup {
		auto fourPcPyroCond = IsActive("viridescentPyro");
		auto fourPcHydroCond = IsActive("viridescentHydro");
		auto fourPcCryoCond = IsActive("viridescentCryo");
		auto fourPcElectroCond = IsActive("viridescentElectro");

		auto fourPcPyroRes = Requires(fourPcPyroCond, Constant(-0.4f));
		auto fourPcHydroRes = Requires(fourPcHydroCond, Constant(-0.4f));
		auto fourPcCryoRes = Requires(fourPcCryoCond, Constant(-0.4f));
		auto fourPcElectroRes = Requires(fourPcElectroCond, Constant(-0.4f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.anemo{.DMG = Constant(0.15f)}},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "viridescentPyro",
						.name = "After triggering a Pyro Swirl reaction",
						.mods{.enemy{.resistance{.pyro = fourPcPyroRes}}},
					},
					Option::Boolean{
						.key = "viridescentHydro",
						.name = "After triggering a Hydro Swirl reaction",
						.mods{.enemy{.resistance{.hydro = fourPcHydroRes}}},
					},
					Option::Boolean{
						.key = "viridescentCryo",
						.name = "After triggering a Cryo Swirl reaction",
						.mods{.enemy{.resistance{.cryo = fourPcCryoRes}}},
					},
					Option::Boolean{
						.key = "viridescentElectro",
						.name = "After triggering a Electro Swirl reaction",
						.mods{.enemy{.resistance{.electro = fourPcElectroRes}}},
					},
				},
				.mods{
					.preMod{
						.swirl{.DMG = Constant(0.6f)},
					},
					.enemy{
						.resistance{
							.pyro = fourPcPyroRes,
							.hydro = fourPcHydroRes,
							.cryo = fourPcCryoRes,
							.electro = fourPcElectroRes,
						},
					},
				},
			},
		};
	},
};
