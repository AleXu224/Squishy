#pragma once

#include "formula/percentage.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "modifiers/talentFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/sheet.hpp"
#include "stats/value.hpp"


namespace Modifiers::Character {
	template<SheetMember<Stats::Sheet<Stats::Value<float, 1>>> stat, SheetMemberIdentifier member>
	struct InstanceStats {
		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Character Base", eval(context), member.isPercentage());
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			return stat.resolve(context.target.character.sheet.stats).get(context);
		}
	};

	template<TalentMember<Talents<Stats::Value<uint32_t, 1>>> stat>
	struct InstanceTalents {
		[[nodiscard]] std::string print(const Formula::Context &, Formula::Step) const {
			// FIXME:: add this
			return "";
		}

		[[nodiscard]] constexpr uint32_t eval(const Formula::Context &context) const {
			return stat.resolve(context.target.character.sheet.talents).get(context);
		}
	};

	static constexpr Modifiers::StatFactory<InstanceStats, StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{}> instanceStats{};
	static constexpr Modifiers::TalentFactory<InstanceTalents, TalentPointerFactory<Talents<Stats::Value<uint32_t, 1>>>{}> instanceTalents{};
}// namespace Modifiers::Character