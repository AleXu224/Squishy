#include "instance.hpp"
#include "formula/percentage.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"
#include "stats/enemy.hpp"
#include "stats/value.hpp"

namespace Modifiers::Enemy {
	struct InstanceStats : Formula::FormulaBase<float> {
		SheetMember<Stats::EnemySheet<Stats::Value<float, 0>>> stat;
		SheetMemberIdentifier member;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return stat.resolve(context.enemy.sheet).fold(context, args);
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