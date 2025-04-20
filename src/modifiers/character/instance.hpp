#pragma once

#include "formula/percentage.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "modifiers/talentFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/sheet.hpp"
#include "stats/value.hpp"


namespace Modifiers::Character {
	template<SheetMember<Stats::Sheet<Stats::Value<float, 2>>> stat, SheetMemberIdentifier member>
	struct InstanceStats {
		[[nodiscard]] auto compile(const Formula::Context &context) const {
			return stat.resolve(context.source.character.sheet.stats).compile(context);
		}
		
		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Character Base", eval(context), member.isPercentage());
		}

		[[nodiscard]] constexpr float eval(const Formula::Context &context) const {
			return stat.resolve(context.source.character.sheet.stats).get(context);
		}
	};

	template<TalentMember<Talents<Stats::Value<uint32_t, 1>>> stat>
	struct InstanceTalents {
		[[nodiscard]] auto compile(const Formula::Context &context) const {
			return stat.resolve(context.source.character.sheet.talents).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &, Formula::Step) const {
			// FIXME:: add this
			return "";
		}

		[[nodiscard]] constexpr uint32_t eval(const Formula::Context &context) const {
			return stat.resolve(context.source.character.sheet.talents).get(context);
		}
	};

	static constexpr Modifiers::StatFactory<InstanceStats, StatPointerFactory<Stats::Sheet<Stats::Value<float, 2>>>{}, StatNameFactory{}> instanceStats{};
	static constexpr Modifiers::TalentFactory<InstanceTalents, TalentPointerFactory<Talents<Stats::Value<uint32_t, 1>>>{}> instanceTalents{};
}// namespace Modifiers::Character