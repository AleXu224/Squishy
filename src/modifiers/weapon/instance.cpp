#include "instance.hpp"
#include "formula/percentage.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/value.hpp"

namespace Modifiers::Weapon {
	struct InstanceStats {
		SheetMember<Stats::Sheet<Stats::Value<float, 1>>> stat;
		SheetMemberIdentifier member;
		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			return stat.resolve(context.source.weapon->sheet.stats).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Weapon Base", eval(context), member.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(context.source.weapon->sheet.stats).get(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &instanceStats() {
		static auto ret = statFactory<Formula::FloatNode, InstanceStats>(StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{});
		return ret;
	}
}// namespace Modifiers::Weapon