#include "instance.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/value.hpp"

namespace Modifiers::Weapon {
	struct InstanceStats : Formula::FormulaBase<float> {
		SheetMember<Stats::Sheet<Stats::Value<float, 1>>> stat;
		SheetMemberIdentifier member;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return stat.resolve(context.source.loadout().weapon->sheet.stats).fold(context, args);
		}

		void print(Formula::Descriptor &descriptor, const Formula::Context &context, Formula::Step) const {
			descriptor.add(std::format("Weapon Base {}", member.getName()), {eval(context), member.isPercentage()});
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return stat.resolve(context.source.loadout().weapon->sheet.stats).get(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &instanceStats() {
		static auto ret = statFactory<Formula::FloatNode, InstanceStats>(StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{});
		return ret;
	}
}// namespace Modifiers::Weapon