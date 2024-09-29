#pragma once

#include "formula/percentage.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/sheet.hpp"
#include "stats/value.hpp"


namespace Modifiers::Weapon {
	template<SheetMember<Stats::Sheet<Stats::Value<float, 1>>> stat, SheetMemberIdentifier member>
	struct InstanceStats {
		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Weapon Base", eval(context), member.isPercentage());
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			return stat.resolve(context.target.weapon.sheet.stats).get(context);
		}
	};

	static constexpr Modifiers::StatFactory<InstanceStats, StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{}> instanceStats{};
}// namespace Modifiers::Weapon