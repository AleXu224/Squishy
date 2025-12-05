#pragma once

#include "fmt/core.h"
#include "formula/formulaContext.hpp"
#include "formula/step.hpp"
#include "misc/reaction.hpp"
#include "misc/skillStat.hpp"
#include "modifiers/total/total.hpp"
#include "stats/helpers.hpp"
#include "utility"

namespace Formula {
	template<Misc::SkillStat skillStat>
	struct ReactionStat {
		Misc::Reaction reaction;

		[[nodiscard]] constexpr const FloatNode &get() const {
			switch (reaction) {
				case Misc::Reaction::vape:
					return Stats::fromSkillStat(Modifiers::total().vape, skillStat);
				case Misc::Reaction::melt:
					return Stats::fromSkillStat(Modifiers::total().melt, skillStat);
				case Misc::Reaction::aggravate:
					return Stats::fromSkillStat(Modifiers::total().aggravate, skillStat);
				case Misc::Reaction::spread:
					return Stats::fromSkillStat(Modifiers::total().spread, skillStat);
				case Misc::Reaction::burning:
					return Stats::fromSkillStat(Modifiers::total().burning, skillStat);
				case Misc::Reaction::superconduct:
					return Stats::fromSkillStat(Modifiers::total().superconduct, skillStat);
				case Misc::Reaction::swirl:
					return Stats::fromSkillStat(Modifiers::total().swirl, skillStat);
				case Misc::Reaction::electroCharged:
					return Stats::fromSkillStat(Modifiers::total().electroCharged, skillStat);
				case Misc::Reaction::shattered:
					return Stats::fromSkillStat(Modifiers::total().shattered, skillStat);
				case Misc::Reaction::overloaded:
					return Stats::fromSkillStat(Modifiers::total().overloaded, skillStat);
				case Misc::Reaction::bloom:
					return Stats::fromSkillStat(Modifiers::total().bloom, skillStat);
				case Misc::Reaction::burgeon:
					return Stats::fromSkillStat(Modifiers::total().burgeon, skillStat);
				case Misc::Reaction::hyperbloom:
					return Stats::fromSkillStat(Modifiers::total().hyperbloom, skillStat);
				case Misc::Reaction::lunarCharged:
					return Stats::fromSkillStat(Modifiers::total().lunarCharged, skillStat);
				case Misc::Reaction::lunarBloom:
					return Stats::fromSkillStat(Modifiers::total().lunarBloom, skillStat);
				case Misc::Reaction::lunarCrystalize:
					return Stats::fromSkillStat(Modifiers::total().lunarCrystallize, skillStat);
			}
			std::unreachable();
		}

		[[nodiscard]] constexpr auto compile(const Context &context) const {
			return get().compile(context);
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("Reaction Bonus {:.1f}", eval(context));
		}

		[[nodiscard]] constexpr float eval(const Context &context) const {
			return get().eval(context);
		}
	};
}// namespace Formula