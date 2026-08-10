#include "instance.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"
#include "stats/enemy.hpp"
#include "stats/value.hpp"

namespace Modifiers::Enemy {
	struct InstanceStats : Formula::FormulaBase<float> {
		EnemyMember<Stats::EnemySheet<Stats::Value<float, 0>>> stat;
		SheetMemberIdentifier member;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return stat.resolve(context.enemy.sheet).fold(context, args);
		}

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.add(std::format("Enemy Base {}", member.getName()), {eval(context), member.isPercentage()});
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