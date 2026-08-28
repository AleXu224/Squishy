#include "KingOfTheSummit.hpp"

#include "disc/setup.hpp"

const Disc::Set Disc::Sets::kingOfTheSummit{
	.key{33200},
	.goodKey{"KingOfTheSummit"},
	.name = "King of the Summit",
	.setup = []() -> Set::Setup {
		auto stunCond = IsAgentSpecialty{.specialty = Misc::Specialty::stun};

		auto cond = IsActive("kingOfTheSummitCond");
		auto buffCd = Requires{
			.requirement = stunCond && cond,
			.ret = Constant{.value = 0.15f},
		};

		auto buffAdditionalCd = Requires{
			.requirement = stunCond && cond && combat.cr >= 0.5f,
			.ret = Constant{.value = 0.15f},
		};

		return Set::Setup{
			.twoPc{
				.mods{
					.initial{
						.all{
							.enemy{
								.daze = Constant{.value = 0.06f},
							},
						},
					},
				},
			},
			.fourPc{
				.opts{
					Option::Boolean{
						.key = "kingOfTheSummitCond",
						.name = "After the equipper uses an EX Special Attack or Chain Attack",
						.teamBuff = true,
						.displayCondition = stunCond,
						.mods{
							.teamCombat{
								.cd = buffCd + buffAdditionalCd,
							},
						},
					},
				},
				.mods{
					.teamCombat{
						.cd = buffCd + buffAdditionalCd,
					},
				},
			},
		};
	},
};
