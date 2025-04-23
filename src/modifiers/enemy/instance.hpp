#pragma once

#include "formula/percentage.hpp"
#include "modifiers/enemyFactory.hpp"
#include "modifiers/helpers.hpp"
#include "stats/enemy.hpp"
#include "stats/sheet.hpp"
#include "stats/value.hpp"


namespace Modifiers::Enemy {
	template<SheetMember<Stats::EnemySheet<Stats::Value<float, 0>>> stat, SheetMemberIdentifier member>
	struct InstanceStats {
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return stat.resolve(context.enemy.sheet).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Enemy", eval(context), member.isPercentage());
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			return stat.resolve(context.enemy.sheet).get(context);
		}
	};

	static constexpr Modifiers::EnemyFactory<InstanceStats, EnemyPointerFactory<Stats::EnemySheet<Stats::Value<float, 0>>>{}, EnemyNameFactory{}> instanceStats{};
}// namespace Modifiers::Enemy