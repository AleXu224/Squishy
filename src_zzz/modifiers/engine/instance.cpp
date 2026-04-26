#include "instance.hpp"
#include "formula/percentage.hpp"
#include "modifiers/helpers.hpp"
#include "modifiers/statFactory.hpp"
#include "stats/loadout.hpp"
#include "stats/value.hpp"

namespace Modifiers::Engine {
	template<Stats::Sheet<Stats::Value<float, 1>> Stats::EngineSheet::*location>
	struct InstanceStats : Formula::FormulaBase<float> {
		SheetMember<Stats::Sheet<Stats::Value<float, 1>>> stat;
		SheetMemberIdentifier member;
		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			if (!context.source.loadout().engine) return Formula::ConstantBase<float>{};
			return stat.resolve(std::invoke(location, context.source.loadout().engine->sheet)).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage("Engine Base", eval(context), member.isPercentage());
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			if (!context.source.loadout().engine) return 0.f;
			return stat.resolve(std::invoke(location, context.source.loadout().engine->sheet)).get(context);
		}
	};

	const Stats::Sheet<Formula::FloatNode> &instanceBase() {
		static auto ret = statFactory<Formula::FloatNode, InstanceStats<&Stats::EngineSheet::base>>(StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{});
		return ret;
	}

	const Stats::Sheet<Formula::FloatNode> &instanceInitial() {
		static auto ret = statFactory<Formula::FloatNode, InstanceStats<&Stats::EngineSheet::initial>>(StatPointerFactory<Stats::Sheet<Stats::Value<float, 1>>>{}, StatNameFactory{});
		return ret;
	}
}// namespace Modifiers::Engine