#include "ArchaicPetra.hpp"

#include "artifact/setup.hpp"

const Artifact::Set Artifact::Sets::archaicPetra{
	.key{15014},
	.goodKey{"ArchaicPetra"},
	.name = "Archaic Petra",
	.setup = []() -> Set::Setup {
		auto fourPcPyro = IsActive("archaicPetraPyro");
		auto fourPcHydro = IsActive("archaicPetraHydro");
		auto fourPcCryo = IsActive("archaicPetraCryo");
		auto fourPcElectro = IsActive("archaicPetraElectro");

		auto fourPcPyroBuff = Requires(fourPcPyro, Constant(0.35f));
		auto fourPcHydroBuff = Requires(fourPcHydro, Constant(0.35f));
		auto fourPcCryoBuff = Requires(fourPcCryo, Constant(0.35f));
		auto fourPcElectroBuff = Requires(fourPcElectro, Constant(0.35f));

		return Set::Setup{
			.twoPc{
				.mods{
					.preMod{.geo{.DMG = Constant(0.15f)}},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "archaicPetraPyro",
						.name = "After obtaining a Pyro Crystallize Shard",
						.displayCondition = !fourPcHydro && !fourPcCryo && !fourPcElectro,
						.mods{
							.preMod{
								.pyro{.DMG = fourPcPyroBuff},
							},
						},
					},
					Option::Boolean{
						.key = "archaicPetraHydro",
						.name = "After obtaining a Hydro Crystallize Shard",
						.displayCondition = !fourPcPyro && !fourPcCryo && !fourPcElectro,
						.mods{
							.preMod{
								.hydro{.DMG = fourPcHydroBuff},
							},
						},
					},
					Option::Boolean{
						.key = "archaicPetraCryo",
						.name = "After obtaining a Cryo Crystallize Shard",
						.displayCondition = !fourPcPyro && !fourPcHydro && !fourPcElectro,
						.mods{
							.preMod{
								.cryo{.DMG = fourPcCryoBuff},
							},
						},
					},
					Option::Boolean{
						.key = "archaicPetraElectro",
						.name = "After obtaining a Electro Crystallize Shard",
						.displayCondition = !fourPcPyro && !fourPcHydro && !fourPcCryo,
						.mods{
							.preMod{
								.electro{.DMG = fourPcElectroBuff},
							},
						},
					},
				},
				.mods{
					.preMod{
						.pyro{.DMG = fourPcPyroBuff},
						.hydro{.DMG = fourPcHydroBuff},
						.cryo{.DMG = fourPcCryoBuff},
						.electro{.DMG = fourPcElectroBuff},
					},
				},
			},
		};
	},
};
