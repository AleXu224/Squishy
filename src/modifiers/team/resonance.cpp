#include "resonance.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/team.hpp"
#include "stats/value.hpp"

namespace Modifiers::Team {
	struct ResonanceFrm {
		SheetMember<Stats::Sheet<Stats::Value<float, 1>>> stat;
		SheetMemberIdentifier member;
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return stat.resolve(context.team.resonances).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Artifact Substat", eval(context), member.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(context.team.resonances).get(context);
		}
	};
	struct EnemyResonanceFrm {
		SheetMember<Stats::EnemySheet<Stats::Value<float, 1>>> stat;
		SheetMemberIdentifier member;
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return stat.resolve(context.team.resonancesEnemy).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Artifact Substat", eval(context), member.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(context.team.resonancesEnemy).get(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &resonances() {
		static auto ret = statFactory<Formula::FloatNode, ResonanceFrm>(StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{});
		return ret;
	}
	const Stats::EnemySheet<Formula::FloatNode> &resonancesEnemy() {
		static auto ret = enemyFactory<Formula::FloatNode, EnemyResonanceFrm>(EnemyPointerFactory<Stats::EnemySheet<Stats::Value<float, 1>>>{}, EnemyNameFactory{});
		return ret;
	}
}// namespace Modifiers::Team