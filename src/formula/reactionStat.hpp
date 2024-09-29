#pragma once

#include "fmt/core.h"
#include "formula/formulaContext.hpp"
#include "formula/step.hpp"
#include "misc/reaction.hpp"
#include "misc/skillStat.hpp"
#include "modifiers/total/total.hpp"
#include "stats/helpers.hpp"


namespace Formula {
	template<Misc::SkillStat skillStat>
	struct ReactionStat {
		Misc::Reaction reaction;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Reaction Bonus {:.1f}", eval(context));
		}

		[[nodiscard]] constexpr float eval(const Context &context) const {
			switch (reaction) {
				case Misc::Reaction::vape:
					return Stats::fromSkillStat<Modifiers::total.vape, skillStat>().eval(context);
				case Misc::Reaction::melt:
					return Stats::fromSkillStat<Modifiers::total.melt, skillStat>().eval(context);
				case Misc::Reaction::aggravate:
					return Stats::fromSkillStat<Modifiers::total.aggravate, skillStat>().eval(context);
				case Misc::Reaction::spread:
					return Stats::fromSkillStat<Modifiers::total.spread, skillStat>().eval(context);
				case Misc::Reaction::burning:
					return Stats::fromSkillStat<Modifiers::total.burning, skillStat>().eval(context);
				case Misc::Reaction::superconduct:
					return Stats::fromSkillStat<Modifiers::total.superconduct, skillStat>().eval(context);
				case Misc::Reaction::swirl:
					return Stats::fromSkillStat<Modifiers::total.swirl, skillStat>().eval(context);
				case Misc::Reaction::electroCharged:
					return Stats::fromSkillStat<Modifiers::total.electroCharged, skillStat>().eval(context);
				case Misc::Reaction::shattered:
					return Stats::fromSkillStat<Modifiers::total.shattered, skillStat>().eval(context);
				case Misc::Reaction::overloaded:
					return Stats::fromSkillStat<Modifiers::total.overloaded, skillStat>().eval(context);
				case Misc::Reaction::bloom:
					return Stats::fromSkillStat<Modifiers::total.bloom, skillStat>().eval(context);
				case Misc::Reaction::burgeon:
					return Stats::fromSkillStat<Modifiers::total.burgeon, skillStat>().eval(context);
				case Misc::Reaction::hyperbloom:
					return Stats::fromSkillStat<Modifiers::total.hyperbloom, skillStat>().eval(context);
			}
		}
	};
}// namespace Formula