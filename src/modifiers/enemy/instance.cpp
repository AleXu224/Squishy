#include "instance.hpp"
#include "formula/percentage.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"
#include "stats/enemy.hpp"
#include "stats/value.hpp"

namespace Modifiers::Enemy {
	struct InstanceStats {
		SheetMember<Stats::EnemySheet<Stats::Value<float, 0>>> stat;
		SheetMemberIdentifier member;
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return stat.resolve(context.enemy.sheet).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Enemy", eval(context), member.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(context.enemy.sheet).get(context);
		}
	};

	const Stats::EnemySheet<Formula::FloatNode> &instanceStats() {
		static auto ret = enemyFactory<Formula::FloatNode, InstanceStats>(EnemyPointerFactory<Stats::EnemySheet<Stats::Value<float, 0>>>{}, EnemyNameFactory{});
		return ret;
	}
}// namespace Modifiers::Enemy